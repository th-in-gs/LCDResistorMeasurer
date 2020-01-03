#include <Arduino.h> 
#include <util/atomic.h>
#include <movingAvg.h>

#include "LCDDisplay.h"
#include "ADCTables.h"


static LCDDisplay display;


static movingAvg ADCReading(10);
static volatile uint8_t ADCReadCount = 0;

void setup()
{
    ADCReading.begin();
    
    display.SetUp();  
    display.SetDigits(-1);

    // Pins 0-4 are used to select test resistor. Leave all at high impedence.
    /*sbi(DDRC, 0);
    cbi(PORTC, 0);

    sbi(DDRC, 1);
    cbi(PORTC, 1);

    sbi(DDRC, 2);
    cbi(PORTC, 2);

    sbi(DDRC, 3);
    cbi(PORTC, 3);

    sbi(DDRC, 4);
    cbi(PORTC, 4);*/

    // Pin 5 is the input ADC pin.  
    cbi(DDRC, 5);
    cbi(PORTC, 5);
    
    ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
  	ADMUX = 0b00000101;

    ADCSRA |= 1<<ADSC;		// Start Conversion
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
    
static const ADCTable ADCTables[] = {
    { E96Values, E96ADCValues_200Ohm, E96ValuesCount, 1 },
    { E96Values, E96ADCValues_221Ohm, E96ValuesCount, 10 },
    { E96Values, E96ADCValues_200Ohm, E96ValuesCount, 100 },
    { E96Values, E96ADCValues_200Ohm, E96ValuesCount, 1000 },
    { E96Values, E96ADCValues_200Ohm, E96ValuesCount, 10000 },
    { E96Values, E96ADCValues_200Ohm, E96ValuesCount, 100000 },
};
static const int ADCTablesLength = sizeof(ADCTables) / sizeof(ADCTables[0]);
    
void loop()
{
    static int ADCTableIndex = ADCTablesLength - 1;
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
        uint8_t readCount;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            sampledADCValue = ADCReading.getAvg();
            readCount = ADCReadCount;
        }
        if(readCount >= 20) {
            ADCValueValid = true;
            
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
            const uint16_t prevValue = pgm_read_word_near((uint16_t *)element - 1);
            const uint16_t value = pgm_read_word_near(element);
            const uint16_t nextValue = pgm_read_word_near((uint16_t *)element + 1);
            
            if(sampledADCValue <= (value - (value - prevValue) / 2)) {
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
        
        if(!tryNewADCTable) {
            display.ClearSymbol(LCDDisplay::LEDSymbol::Colon);
            display.ClearSymbol(LCDDisplay::LEDSymbol::DecimalPoint);
            display.ClearSymbol(LCDDisplay::LEDSymbol::DownArrowOnRight);
            display.ClearSymbol(LCDDisplay::LEDSymbol::UpArrowOnRight);

            display.SetSymbol(LCDDisplay::LEDSymbol::DottedUpsidedownT);
            display.SetSymbol(LCDDisplay::LEDSymbol::ChartGoingNegative);
            tryNewADCTable = true;
        }
    } else {        
        uint32_t resistorValue;
        if((millis() % 1000) < 250) {
            resistorValue = sampledADCValue;
        } else {
            resistorValue = displayValue * ADCTables[ADCTableIndex].valueMultiplier;
        }
        
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
        
        if(tryNewADCTable) {
            display.ClearSymbol(LCDDisplay::LEDSymbol::DottedUpsidedownT);
            display.ClearSymbol(LCDDisplay::LEDSymbol::ChartGoingNegative);
            tryNewADCTable = false;
        }
    }
}


ISR(ADC_vect)
{
    uint16_t thisReading = ADC;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ADCReading.reading(thisReading);
        ++ADCReadCount;
    }

	ADCSRA |= 1<<ADSC;		// Start Conversion
}
