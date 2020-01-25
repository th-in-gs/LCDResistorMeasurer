#include <Arduino.h> 
#include <util/atomic.h>

#include "LCDDisplay.h"
#include "ADCTables.h"
#include "Conversions.h"

static LCDDisplay display;

static volatile uint16_t ADCReadings[128];
static const auto ADCReadingBufferLength = sizeof(ADCReadings) / sizeof(ADCReadings[0]);

static volatile uint16_t ADCReadCount = 0;

void setup()
{
    display.SetUp();  

    // Bits 0-4 of PORTC are used to select test resistor.
    // Leave as high-impedance, external pull-ups will pull PNP bases to V+

    // Bit 5 is used for ADC input. Leave it as input high-impedance
        
    // Set up ADC options. Slowest conversion, interrupts.
    ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    
    // AVCC with external capacitor at AREF pin, ADC 5 selected. 
  	ADMUX = 0b01000101;

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

struct ADCTable {
    const uint32_t referenceResistorOhms;
};

static const ADCTable ADCTables[] = { // Values before multiplication are in ohms/10.
    { 200 },
    { 2210 },
    { 20000 },
    { 200000 },
    { 2000000 },
};
static const auto ADCTablesLength = sizeof(ADCTables) / sizeof(ADCTables[0]);
    
void loop()
{ 
    // Rotate through values to find one in 233 - 787 range.
    // If we're at the first table, use 0 - 787 range.
    // If we're at the last table, use 233 - 787 range. 

    static uint8_t ADCTableIndex = ADCTablesLength - 1;
    static uint8_t nextADCTableIndex = 0;

    if(nextADCTableIndex != ADCTableIndex) {
        // Old port back to input, high impedence.
        cbi(DDRC, ADCTableIndex);
        
        ADCTableIndex = nextADCTableIndex;
        
        // New port to output, 0 to switch on PNP transistor.
        sbi(DDRC, ADCTableIndex);
        
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            ADCReadCount = 0;
        }
    }

    uint16_t sampledADCValue;
    bool ADCValueValid = false;
    do {    
        uint16_t copiedReadings[ADCReadingBufferLength];
        uint16_t copiedReadCount;
        
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            for(uint8_t i = 0; i < ADCReadingBufferLength; ++i) {
                copiedReadings[i] = ADCReadings[i];
            }
            copiedReadCount = ADCReadCount;
        }
        
        if(copiedReadCount >= ADCReadingBufferLength * 2) {
            ADCValueValid = true;
            
            // Find the median.
            qsort(
                copiedReadings, 
                sizeof(copiedReadings) / sizeof(copiedReadings[0]), 
                sizeof(copiedReadings[0]),
                [](const void *pLhs, const void *pRhs) {
                  typeof(copiedReadings[0]) lhs = *((typeof(copiedReadings[0]) *)pLhs);
                  typeof(copiedReadings[0]) rhs = *((typeof(copiedReadings[0]) *)pRhs);
                  return ((int)lhs - (int)rhs);
                }
            );
            sampledADCValue = copiedReadings[(sizeof(copiedReadings) / sizeof(copiedReadings[0])) / 2];
        } 
    } while(!ADCValueValid);
    
    // Check if we're in the 'good' range (with good resolution),
    // try another table next time if not.
    if(sampledADCValue < 233 && ADCTableIndex != 0) {
        nextADCTableIndex = ADCTableIndex - 1;
    } else if (sampledADCValue > 787 && ADCTableIndex < (ADCTablesLength - 1)) {
        nextADCTableIndex = ADCTableIndex + 1;
    }
                
    uint32_t resistorValue = ADCToOhms(sampledADCValue, ADCTables[ADCTableIndex].referenceResistorOhms);    
        
    if(sampledADCValue >= 950 || resistorValue <= 5) {
        display.SetDigits(-1);
        
        display.ClearSymbol(LCDDisplay::LEDSymbol::Colon);
        display.ClearSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
        display.ClearSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
        display.ClearSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);

        display.SetSymbol(LCDDisplay::LEDSymbol::DottedUpsidedownT);
        display.SetSymbol(LCDDisplay::LEDSymbol::ChartGoingNegative);
    } else {   
        display.ClearSymbol(LCDDisplay::LEDSymbol::DottedUpsidedownT);
        display.ClearSymbol(LCDDisplay::LEDSymbol::ChartGoingNegative);
        
        char digitString[11];
        int8_t digitsLength = sprintf(digitString, "%" PRIu32, resistorValue);


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

        static const uint16_t *valuesTable;
        static uint8_t valuesTableCount;
        
        static_assert(sizeof(E96Values[0]) == sizeof(valuesTable[0]), "Expecting 16-bit values");

        // Select the appropriate lookup table.
        valuesTable = E96Values;
        valuesTableCount = E96ValuesCount;
        
        // Use the C bsearch routine (a little unorthodoxly) to find the
        // nearest table value.
        const size_t nearestElementIndex = (size_t)bsearch(
            &leading4Digits,
            0, // This takes a pointer really, but we're coercing it to behave like it takes indexes.
            valuesTableCount,
            1, // Size is also 1 to coerce index-like behaviour.
            [](const void *key, const void *element) {
                const uint16_t resistorValue = *((uint16_t *)key);
                const size_t index = (uint8_t)(size_t)element;
                
                // Treat each entry in the ADC table as a 'bucket' stretching
                // between the two adjacent values.
                
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
                
                if(resistorValue == value) {
                    return 0;
                } else if(resistorValue <= (value - (value - prevValue) / 2)) {
                    return -1;
                } else if(resistorValue > (nextValue - (nextValue - value) / 2)) {
                    return 1;
                } else {
                    return 0;
                }
            }
        );
        
        uint32_t tableResistorValue = (uint32_t)pgm_read_word_near(valuesTable + nearestElementIndex);
        
        // Pad the value from the table with the same number of places that 
        // we cut off the original value (or cut off however many we padded
        // it by).
        while(remainingPlaces < 0) {
            tableResistorValue /= 10;
            remainingPlaces += 1;
        } 
        while(remainingPlaces > 0) {
            tableResistorValue *= 10;
            remainingPlaces -= 1;
        }
        
        digitsLength = sprintf(digitString, "%" PRIu32, tableResistorValue);

        uint16_t displayNumber = 0;        
        for(int8_t i = 0; i < digitsLength && i < 3; ++i) {
            displayNumber = displayNumber * 10 + (digitString[i] - '0');
        }    

        display.SetDigits(displayNumber);
        
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
                // Error
                display.SetSymbol(LCDDisplay::LEDSymbol::Colon);
                display.SetSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
                display.SetSymbol(LCDDisplay::LEDSymbol::CautionTriangle);
                break;
            }
        }            
         
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
                display.SetSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
                display.SetSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);
                display.SetSymbol(LCDDisplay::LEDSymbol::TriangleInCircle);
                break;
            }
        }        
    }
}

ISR(ADC_vect)
{
    static uint8_t ADCReadingBufferCursor = 0;
    uint16_t thisReading = ADC;
          
    ADCReadingBufferCursor = (ADCReadingBufferCursor + 1) % ADCReadingBufferLength;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ADCReadings[ADCReadingBufferCursor] = thisReading;
        ++ADCReadCount;
    }
    
	ADCSRA |= 1<<ADSC;		// Start Conversion
}
