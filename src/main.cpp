#include <Arduino.h> 
#include <util/atomic.h>
 
#include "LCDDisplay.h"
#include "ADCTables.h"
#include "Conversions.h"

static LCDDisplay display;

static const uint16_t ADCReadingBufferLength = 128;

static volatile uint16_t ADCReadings[ADCReadingBufferLength];
static volatile uint16_t ADCReadCount;

void setup()
{        
    // Set up AVCC with external capacitor atm AREF pin, ADC 5 selected. 
  	ADMUX = 0b01000101;

    display.SetUp();  

    // Bits 0-4 of PORTC are used to select test resistor.
    // Leave as high-impedance, external pull-ups will pull PNP bases to V+

    // Bit 5 is used for ADC input. Leave it as input high-impedance

    // Set up ADC options. Slowest conversion, interrupts.
    ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

    // Start Conversion
    ADCSRA |= 1<<ADSC;		
}

/*

E24	200 	10	    1000     *  10  - 1K
	2000	100	    10000       100 - 10K 
	20000	1000	100000   *  1K  - 100K
	200000	10000	1000000  ?  10K - 1M
	
			
E96	200	    100	    1000        100Ω - 1K   *  200      200Ω
	2000	1000	10000         1K - 10K  *  2200       2.2K
	20000	10000	100000       10K - 100K *  20000     20K
	200000	100000	1000000     100K - 1M   *  200000   200K
  	2000000 1000000 10000000      1M - 10M  *  2000000    2M

*/

/*
extern char *__brkval;

static int freeMemory() {
  char top;
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
}
*/

static const uint32_t ReferenceResistorValues[] = {
    200,
    2210, // Ideal would be 2000 here, but I only had a precision 2210 resistor!
    20000,
    200000,
    2000000 
};
static const auto ReferenceResistorValuesLength = sizeof(ReferenceResistorValues) / sizeof(ReferenceResistorValues[0]);
    
void loop()
{ 
    static unsigned long lineHeldLowTime = 0;

    static uint8_t referenceResistorValueIndex = ReferenceResistorValuesLength - 1;
    static uint8_t nextReferenceResistorValueIndex = 0;

    static uint8_t eValuesIndex = 0;

    // Rotate through values to find one in 233 - 787 range.
    // If we're at the first table, use 0 - 787 range.
    // If we're at the last table, use 233 - 787 range. 
    // [The actual calculation of nextReferenceResistorValueIndex was
    // performed on the previous iteration of the loop, after the ADC
    // value was read - see "Check if we're in the 'good' range" comment
    // a bit lower].
    if(nextReferenceResistorValueIndex != referenceResistorValueIndex) {
        /*
        // Debug code - show which table is being used on the top row of the 
        // LCD.
        static const LCDDisplay::LEDSymbol symbols[] = {
            LCDDisplay::LEDSymbol::One,
            LCDDisplay::LEDSymbol::Radiator,
            LCDDisplay::LEDSymbol::Two,
            LCDDisplay::LEDSymbol::Tap,
            LCDDisplay::LEDSymbol::TriangleInCircle,
        };
        
        display.ClearSymbol(symbols[referenceResistorValueIndex]);
        display.SetSymbol(symbols[nextReferenceResistorValueIndex]);
        */

        // Old port back to input, high impedance.
        cbi(DDRC, referenceResistorValueIndex);
        
        referenceResistorValueIndex = nextReferenceResistorValueIndex;
        
        // New port to output, 0 to switch on PNP transistor.
        sbi(DDRC, referenceResistorValueIndex);
        
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            ADCReadCount = 0;
        }
    }

    // The ADC is always going on in the background, filling up the ADCReadings
    // array - see ISR(ADC_vect).
    // We wait for the readings in the array to settle (might take a short time
    // especially if the referenceResistorValueIndex just changed), then 
    // an average from all the readings int he array.
    uint16_t sampledADCValue;
    bool ADCValueValid = false;
    do {    
        uint16_t copiedReadCount;
        uint32_t accumulatedValues = 0;
        uint16_t minValue = UINT16_MAX;
        uint16_t maxValue = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            for(uint16_t i = 0; i < ADCReadingBufferLength; ++i) {
                uint16_t value = ADCReadings[i];
                accumulatedValues += value;
                if(value < minValue) {
                    minValue = value;
                }
                if(value > maxValue) {
                    maxValue = value;
                }
            }
            copiedReadCount = ADCReadCount;
        }
        
        if(copiedReadCount >= ADCReadingBufferLength * 2 &&
           maxValue - minValue < 256) {
            sampledADCValue = accumulatedValues / ADCReadingBufferLength;
            ADCValueValid = true;
        } 
    } while(!ADCValueValid);
    
    
    // Check if we're in the 'good' range (with good resolution),
    // try another table next time if not.
    if(sampledADCValue < 233 && referenceResistorValueIndex != 0) {
        nextReferenceResistorValueIndex = referenceResistorValueIndex - 1;
    } else if (sampledADCValue > 787 && referenceResistorValueIndex < (ReferenceResistorValuesLength - 1)) {
        nextReferenceResistorValueIndex = referenceResistorValueIndex + 1;
    }
                
                    
    // A switch that closes the test resistor contacts serves as an input (I 
    // ran out of I/O pins!)
    // If it's held for over 1 second, switch the E series we're rounding to.
    bool shouldDisplaySwitchedEValues = false;
    if(sampledADCValue < 90 && referenceResistorValueIndex == 0) {
        unsigned long nowTime = millis();
        if(lineHeldLowTime == 0) {
            lineHeldLowTime = nowTime;
        }
        
        shouldDisplaySwitchedEValues = true;
        
        if((nowTime - lineHeldLowTime) > 750) {
            eValuesIndex = (eValuesIndex + 1) % 3;
            lineHeldLowTime = 0;
        }
    } else {
        lineHeldLowTime = 0;
    }
    
    // Now comes the most complex stuff in the loop - work out how to display
    // the value we just read!
    
    // Light up the arrow that points to the E96 or E24 index when appropriate.
    static uint8_t lastDisplayedEValuesIndex = 2;
    if(eValuesIndex != lastDisplayedEValuesIndex) {
        if(lastDisplayedEValuesIndex < 2) {
            display.ClearSymbol(lastDisplayedEValuesIndex == 0 ? 
                                LCDDisplay::LEDSymbol::DownArrowOnLeft : 
                                LCDDisplay::LEDSymbol::UpArrowOnLeft);
        }
        if(eValuesIndex < 2) {
            display.SetSymbol(eValuesIndex == 0 ? 
                              LCDDisplay::LEDSymbol::DownArrowOnLeft : 
                              LCDDisplay::LEDSymbol::UpArrowOnLeft);
        }
        lastDisplayedEValuesIndex = eValuesIndex;
    } 


    if(shouldDisplaySwitchedEValues) {
        // We just changed the E series we're rounding to - display some
        // feedback on the entire display (instead of the resistor value read).   
        display.ClearSymbol(LCDDisplay::LEDSymbol::Colon);
        display.ClearSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
        display.ClearSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
        display.ClearSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);
        display.ClearSymbol(LCDDisplay::LEDSymbol::MinusLeft);
        display.ClearSymbol(LCDDisplay::LEDSymbol::Plus);
        display.ClearSymbol(LCDDisplay::LEDSymbol::Target);

        display.SetString(((const char *[]){"E24", "E96", "---"})[eValuesIndex]);
    } else {
        // The normal case - display the resistor value we just read.
        
        uint32_t resistorValue = ADCToOhms(sampledADCValue, ReferenceResistorValues[referenceResistorValueIndex]);    
        //uint32_t resistorValue = sampledADCValue;
        
        if(resistorValue >= 1000000000 ||
           sampledADCValue >= 950 ||
           resistorValue <= 5) {
            // Blank display. There's probably no resistor connected.
                   
            display.ClearSymbol(LCDDisplay::LEDSymbol::Colon);
            display.ClearSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
            display.ClearSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
            display.ClearSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);
            display.ClearSymbol(LCDDisplay::LEDSymbol::MinusLeft);
            display.ClearSymbol(LCDDisplay::LEDSymbol::Plus);
            display.ClearSymbol(LCDDisplay::LEDSymbol::Target);
            
            display.SetDigits(-1);
        } else {   
            // Display the resistor value we just read.
            
            uint32_t roundedResistorValue = resistorValue;
            static const uint16_t *valuesTable;
            static uint8_t valuesTableCount;

            // Select the appropriate lookup table.
            if(eValuesIndex == 0) {
                valuesTable = E24Values;
                valuesTableCount = E24ValuesCount;
            } else if(eValuesIndex == 1) {
                valuesTable = E96Values;
                valuesTableCount = E96ValuesCount;
            } else {
                valuesTable = NULL;
                valuesTableCount = 0;
            }

            // If we have a table (E24 or E96), find the value that most
            // closely matches the resistor under test.
            if(valuesTableCount) {
                char digitString[11];
                            
                int8_t digitsLength = i32toa(resistorValue, digitString);
                
                // Make a 4 digit number to look up in our E-Value tables.
                // (The tables are padded to 4 digits, with a 3-digit 'off the bottom'
                // and 5-digit 'off the top' number at the ends to assist with lookup)
                int8_t remainingPlaces = digitsLength;
                uint16_t leading4Digits = 0;
                for(int8_t i = 0; i < digitsLength && i < 4; ++i) {
                    leading4Digits = leading4Digits * 10 + (digitString[i] - '0');
                    remainingPlaces -= 1;
                }    
                if(digitsLength < 4) {
                    for(int8_t i = digitsLength; i < 4; ++i) {
                        leading4Digits *= 10;
                        remainingPlaces -= 1;
                    }
                }

                // Use the C bsearch routine (a little unorthodoxly) to find the
                // nearest table value.
                const size_t nearestElementIndex = (size_t)bsearch(
                    &leading4Digits,  // The 'key' - the value we're searching for.
                    0,                // This is supposed to be a pointer 
                                      // but we're going to use it as an index,
                                      // so we start at index 0.
                    valuesTableCount, // Number of elements in the table.
                    1,                // Size is also 1 to coerce index-like behaviour.
                    [](const void *key, const void *element) {
                        // This is a lambda we're passing in as the comparison 
                        // function for the bsearch routine to use.
                        // It takes a key (which is always actually 
                        // &leading4Digits) and an element pointer (which 
                        // we're using as in index into the values table).
                        
                        // Could just use resistorValue = leading4Digits,
                        // but we'll get it out of the key pointer we're passed
                        // for correctness' sake.
                        const uint16_t resistorValue = *((uint16_t *)key);
                        
                        // The index into the values table we're currently 
                        // consdering.
                        const size_t index = (uint8_t)(size_t)element;
                        
                        // We want to find the value that's _closest_ to the 
                        // the resistorValue - no matter if it's larger or 
                        // smaller. So we treat each entry in the ADC table as 
                        // a 'bucket' stretching between the halfway points
                        // between the indexed value and the previous and next
                        // values in the table.
                    
                        // We need to look up the previous, next, and current value.
                        const uint16_t prevValue = [index]() { 
                            if(index == 0) {
                                return (uint16_t)0;
                            } else {
                                return pgm_read_word_near(valuesTable + index - 1); 
                            }
                        }();
                        const uint16_t value = pgm_read_word_near(valuesTable + index);
                        const uint16_t nextValue = [index]() { 
                            if(index + 1 < valuesTableCount) {
                                return pgm_read_word_near(valuesTable + index + 1);
                            } else {
                                return UINT16_MAX; 
                            }
                        }();
                        
                        if(resistorValue <= (value - (value - prevValue) / 2)) {
                            // resistorValue is not in the bucket, and it's 
                            // smaller than the lower end of the bucket.
                            return -1;
                        } else if(resistorValue > (nextValue - (nextValue - value) / 2)) {
                            // resistorValue is not in the bucket, and it's 
                            // larger than the upper end of the bucket.
                            return 1;
                        } else {
                            // We've got a closest match, se we say it's equal.
                            return 0;
                        }
                    }
                );
                
                // Look up the bsearch-found value from the table.
                roundedResistorValue = (uint32_t)pgm_read_word_near(valuesTable + nearestElementIndex);
                
                // Pad the value from the table by the same number of places 
                // that we cut off the original value (or cut off however many 
                // we padded it by).
                while(remainingPlaces < 0) {
                    roundedResistorValue /= 10;
                    remainingPlaces += 1;
                } 
                while(remainingPlaces > 0) {
                    roundedResistorValue *= 10;
                    remainingPlaces -= 1;
                }
            }
            
            // Display the value.
            
            // We'll work on an ascii representation for ease of dealing with
            // the base-10 number.
            char digitString[11];
            int8_t digitsLength = i32toa(roundedResistorValue, digitString);
            //int8_t digitsLength = i32toa(freeMemory(), digitString);

            // We only display max 3 digits on the LCD, so work out what 
            // these three digits should be and store them in displayNumber
            // (back in normal integer binary representation).
            uint16_t displayNumber = 0;        
            for(int8_t i = 0; i < digitsLength && i < 3; ++i) {
                displayNumber = displayNumber * 10 + (digitString[i] - '0');
            }
            
            // If we had to cut off digits, round the last one correctly.
            if(digitsLength >= 4) {
                if((digitString[3] - '0') >= 5) {
                    ++displayNumber;
                }
            }

            // Display the digits.
            display.SetDigits(displayNumber);
            
            // Now, we need to work out whether to light up the 'k' or 'M' 
            // arrows, and where to place the decimal point if one is 
            // necessary.
            char suffix = '\0';
            uint8_t decimalPlace = 0xff;

            switch(digitsLength) {
                case 1: 
                case 2: 
                case 3: {
                    decimalPlace = 0;
                    suffix = '-';
                    break;
                }
                case 4: {
                    decimalPlace = 2;
                    suffix = 'k';
                    break;
                }
                case 5: {
                    decimalPlace = 1;
                    suffix = 'k';
                    break;
                }
                case 6: {
                    decimalPlace = 0;
                    suffix = 'k';
                    break;
                }
                case 7: {
                    decimalPlace = 2;
                    suffix = 'M';
                    break;
                }
                case 8: {
                    decimalPlace = 1;
                    suffix = 'M';
                    break;
                }
                case 9: {
                    decimalPlace = 0;
                    suffix = 'M';
                    break;
                }
                default: {
                    // Error - will be dealt with below.
                    break;
                }
            }
            
            // Place a decimal point in the right place (either the decimal
            // point symbol for numbers that need a point before the last digit
            // - 00.0 style - or the colon for numbers that need it in the 
            // second-to-last place - 0.00 style (or, really, 0:00, because 
            // that's what our re-purposed LCD can display) 
            switch(decimalPlace) {
                case 0: {
                    display.ClearSymbol(LCDDisplay::LEDSymbol::Colon);
                    display.ClearSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
                    break;
                }
                case 1: {
                    display.ClearSymbol(LCDDisplay::LEDSymbol::Colon);
                    display.SetSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
                    break;
                }
                case 2: {
                    display.SetSymbol(LCDDisplay::LEDSymbol::Colon);
                    display.ClearSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
                    break;
                }
                default: {
                    // Error.
                    display.ClearSymbol(LCDDisplay::LEDSymbol::Colon);
                    display.ClearSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
                    display.SetSymbol(LCDDisplay::LEDSymbol::CautionTriangle);
                    break;
                }
            }            
            
            // Light up the appropriate arrow, pointing wither to 'k' or 'M' - 
            // or no arrow if the unit it just ohms.
            switch(suffix) {
                case '-': {
                    display.ClearSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
                    display.ClearSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);
                    break;
                }
                case 'k': {
                    display.ClearSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
                    display.SetSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);
                    break;
                }
                case 'M': {
                    display.SetSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
                    display.ClearSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);
                    break;
                }
                default: {
                    // Error.
                    display.ClearSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
                    display.ClearSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);
                    display.SetSymbol(LCDDisplay::LEDSymbol::TriangleInCircle);
                    break;
                }
            }
            
            uint32_t difference;
            if(resistorValue > roundedResistorValue) {
                difference = resistorValue - roundedResistorValue;
            } else {
                difference = roundedResistorValue - resistorValue;
            }
            
            // Are we within 5%, for E24, or 1%, for E96? If so, light up the 
            // 'target' symbol.
            // [Actually, are the tables such that this is always true?...]
            uint8_t percentageTimes10 = ((difference * 1000) / roundedResistorValue);
            if(eValuesIndex != 2 &&
               (percentageTimes10 <= 50 && 
               (percentageTimes10 <= 10 || eValuesIndex == 0))) {
                display.SetSymbol(LCDDisplay::LEDSymbol::Target);
            } else {
                display.ClearSymbol(LCDDisplay::LEDSymbol::Target);
            }
            
            //display.SetDigits(percentageTimes10);
            
            if(percentageTimes10 >= 10) {
                // Display if the measured value is higher or lower than the
                // displayed value.
                if(resistorValue > roundedResistorValue) {
                    display.ClearSymbol(LCDDisplay::LEDSymbol::MinusLeft);
                    display.SetSymbol(LCDDisplay::LEDSymbol::Plus);
                } else {
                    display.SetSymbol(LCDDisplay::LEDSymbol::MinusLeft);
                    display.ClearSymbol(LCDDisplay::LEDSymbol::Plus);
                }
            } else {  
                display.ClearSymbol(LCDDisplay::LEDSymbol::MinusLeft);
                display.ClearSymbol(LCDDisplay::LEDSymbol::Plus);
            }
        }
    }
}

ISR(ADC_vect)
{
    static uint16_t ADCReadingBufferCursor = 0;
    uint16_t thisReading = ADC;
          
    ADCReadingBufferCursor = (ADCReadingBufferCursor + 1) % ADCReadingBufferLength;
    const uint16_t postReadCount = min(ADCReadCount + 1, UINT16_MAX - 1);
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ADCReadings[ADCReadingBufferCursor] = thisReading;
        ADCReadCount = postReadCount;
    }
    
	ADCSRA |= 1<<ADSC;		// Start Conversion
}
