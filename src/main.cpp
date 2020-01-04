#include <Arduino.h> 
#include <util/atomic.h>

#include "LCDDisplay.h"
#include "ADCTables.h"


static LCDDisplay display;


static volatile uint16_t ADCReadings[49];
static const auto ADCReadingBufferLength = sizeof(ADCReadings) / sizeof(ADCReadings[0]);

static volatile uint16_t ADCReadCount = 0;

void setup()
{
    display.SetUp();  
    display.SetDigits(-1);

    // Bits 0-4 of PORTC are used to select test resistor. 
    // Bit 5 is used for ADC input.
    
    // Leave all at high impedence (that's the default power-on state
    // so we don't need to set anything up).
    
    // Set up ADC options. Slowest conversion, interrupts.
    ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    
    // ADC 5. 
  	ADMUX = 0b00000101;

    // Start Conversion
    ADCSRA |= 1<<ADSC;		
}

/*
    M = Max ADC value.

    F = fixed resistor.
    T = Test resistor.
    S = Sample / M (so scaled to 0 - 1 range)

    V = Sample = S * M

                         S = T / (F + T) 
               (F + T) * S = T
             F * S + T * S = T
             F * S         = T - T * S
           (F * S) / T     = 1 - S
    ((F * S) / T / (1 - S) = 1
                         T = (F * S) / (1 - S)
                         T = (F * (V / 1023)) / (1 - (V / 1023))
                         T = ((F * V) / (1 - V)) / 1023


  Example: 604 kΩ, 604 kΩ, sampled value 513
    T  = (604 * (513 / 1023)) / (1 - (513 / 1023))

    511: 602.8
    512: 605.1
    513: 607.6
    514: 610.0

    Want at least 2 ADC units per measured unit.

*/

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
    const uint16_t * PROGMEM values;
    const uint16_t * PROGMEM ADCValues;
    const uint8_t valuesCount;
    const uint32_t valueMultiplier;
};
    
static const ADCTable ADCTables[] = { // Values before multiplication are in ohms/10.
    { E96Values, E96ADCValues_200Ohm, E96ValuesCount, 1 },      // 57.6  - 715    715
    { E96Values, E96ADCValues_221Ohm, E96ValuesCount, 10 },     // 575   - 7.15K  7150
    { E96Values, E96ADCValues_200Ohm, E96ValuesCount, 100 },    // 5.76K - 71.5K  71500
    { E96Values, E96ADCValues_200Ohm, E96ValuesCount, 1000 },   // 57.6K - 715K   715000
    { E96Values_Large, E96ADCValues_200Ohm_Large, E96ValuesCount_Large, 10000 },  // 576K  - 7.15M  7150000
};
static const auto ADCTablesLength = sizeof(ADCTables) / sizeof(ADCTables[0]);
    
void loop()
{
    /*
    static uint8_t ADCTableIndex = 4;
    static bool tryNewADCTable = false;
    sbi(DDRC, 4);
    */
    
    static uint8_t ADCTableIndex = ADCTablesLength - 1;
    static bool tryNewADCTable = true;

    if(tryNewADCTable) {
        cbi(DDRC, ADCTableIndex);
        ADCTableIndex = (ADCTableIndex + 1) % ADCTablesLength;
        sbi(DDRC, ADCTableIndex);
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            ADCReadCount = 0;
        }
    }

    uint16_t sampledADCValue;
    bool ADCValueValid = false;
    do {    
        uint16_t copiedReadings[ADCReadingBufferLength];
        uint16_t readCount;
        
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            for(uint8_t i = 0; i < ADCReadingBufferLength; ++i) {
                copiedReadings[i] = ADCReadings[i];
            }
            readCount = ADCReadCount;
        }
        
        if(readCount >= 100) {
            ADCValueValid = true;
            qsort(
                copiedReadings, 
                sizeof(copiedReadings) / sizeof(copiedReadings[0]), 
                sizeof(copiedReadings[0]),
                [](const void *pLhs, const void *pRhs) {
                  typeof(copiedReadings[0]) lhs = *((typeof(copiedReadings[0]) *)pLhs);
                  typeof(copiedReadings[0]) rhs = *((typeof(copiedReadings[0]) *)pRhs);
                  return (int)(lhs - rhs);
                }
            );
            sampledADCValue = copiedReadings[(sizeof(copiedReadings) / sizeof(copiedReadings[0])) / 2];
            
            // Originally designed the circuit with the test and calibration resistors
            // the opposite way around...
            // sampledADCValue = 1024 - sampledADCValue;
        } else {
            delay(16);
        }
    } while(!ADCValueValid);
        
        
    uint16_t displayValue = 0xff;
    
    const uint16_t *nearestElement = (const uint16_t *)bsearch(
        &sampledADCValue,
        ADCTables[ADCTableIndex].ADCValues + 1, 
        ADCTables[ADCTableIndex].valuesCount - 2, 
        sizeof(ADCTables[ADCTableIndex].ADCValues[0]),
        [](const void *key, const void *element) {
            const uint16_t sampledADCValue = *((uint16_t *)key);
            
            // Treat each entry in the ADC table as a 'bucket' stretching 
            // between the two adjacent values.
            const uint16_t prevValue = pgm_read_word_near((uint16_t *)element - 1);
            const uint16_t value = pgm_read_word_near(element);
            const uint16_t nextValue = pgm_read_word_near((uint16_t *)element + 1);
            
            if(sampledADCValue == value) {
                return 0;
            } else if(sampledADCValue <= (value - (value - prevValue) / 2)) {
                return -1;
            } else if(sampledADCValue > (nextValue - (nextValue - value) / 2)) {
                return 1;
            } else {
                return 0;
            }
        }
            
    ); 
    if(nearestElement) {
        int foundValueIndex = nearestElement - ADCTables[ADCTableIndex].ADCValues;
        displayValue = pgm_read_word_near(ADCTables[ADCTableIndex].values + foundValueIndex);
    }

    if(displayValue == 0xff) {
        display.SetDigits(-1);
        
        display.ClearSymbol(LCDDisplay::LEDSymbol::Colon);
        display.ClearSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
        display.ClearSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
        display.ClearSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);

        display.SetSymbol(LCDDisplay::LEDSymbol::DottedUpsidedownT);
        display.SetSymbol(LCDDisplay::LEDSymbol::ChartGoingNegative);
        
        tryNewADCTable = true;
    } else {        
        uint32_t resistorValue;
  /*      if((millis() % 1000) < 250) {
            resistorValue = sampledADCValue * 10;
        } else {
 */           resistorValue = displayValue * ADCTables[ADCTableIndex].valueMultiplier;
 //       }
        
        
        // Values are in ohms/10, have three significant digits, and always have
        // three digits (smallest is currently 57.6 ohms - or 576). 
        
        char digits[11];
        uint8_t digitsLength = snprintf(digits, sizeof(digits), "%" PRIu32, resistorValue);
        if(digitsLength >= sizeof(digits)) {
            digitsLength = sizeof(digits) - 1;
        }
        
        uint16_t displayNumber = 0;
        for(char *digitCursor = digits; *digitCursor != '\0' && digitCursor < digits + 3; ++digitCursor) {
            displayNumber = displayNumber * 10 + (*digitCursor - '0');
        }

        display.SetDigits(displayNumber);
        
        char suffix = '\0';
        uint8_t decimalPlace = 0xff;

        switch(digitsLength) {
            case 3: {
                decimalPlace = 1;
                suffix = '-';
                break;
            }
            case 4: {
                decimalPlace = 0;
                suffix = '-';
                break;
            }
            case 5: {
                decimalPlace = 2;
                suffix = 'k';
                break;
            }
            case 6: {
                decimalPlace = 1;
                suffix = 'k';
                break;
            }
            case 7: {
                decimalPlace = 0;
                suffix = 'k';
                break;
            }
            case 8: {
                decimalPlace = 2;
                suffix = 'M';
                break;
            }
            case 9: {
                decimalPlace = 1;
                suffix = 'M';
                break;
            }
            case 10: {
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
        
        if(tryNewADCTable) {
            display.ClearSymbol(LCDDisplay::LEDSymbol::DottedUpsidedownT);
            display.ClearSymbol(LCDDisplay::LEDSymbol::ChartGoingNegative);
            tryNewADCTable = false;
        }
    }
}


ISR(ADC_vect)
{
    static uint8_t cursor = 0;
    uint16_t thisReading = ADC;
          
    cursor = (cursor + 1) % ADCReadingBufferLength;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ADCReadings[cursor] = thisReading;
        ++ADCReadCount;
    }
    
	ADCSRA |= 1<<ADSC;		// Start Conversion
}
