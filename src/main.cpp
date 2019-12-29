#include <Arduino.h> 
#include <util/atomic.h>

#include "LCDDisplay.h"
#include "ADCTables.h"


typedef enum {
    EOutputRangeE3,
    EOutputRangeE6,
    EOutputRangeE12,
    EOutputRangeE24,
  //  EOutputRangeE48,
  //  EOutputRangeE96,
} EOutputRange;


/*
E24	200 	10	    1000     *  10  - 1K
	2000	100	    10000       100 - 10K 
	20000	1000	100000   *  1K  - 100K
	200000	10000	1000000  ?  10K - 1M
	
			
E96	200	    100	    1000        100Ω - 1K
	2000	1000	10000         1K - 10K
	20000	10000	100000       10K - 100K
	200000	100000	1000000     100K - 1M
	2000000 1000000 10000000      1M - 10M
*/


/*
    Have:
        200
        2210
        20000
        200000
*/

typedef enum {
    MeasurementScale10to1000,
    MeasurementScale1000to100000,
} MeasurementScale;

static LCDDisplay display;

void setup()
{
    display.SetUp();  

    sbi(DDRC, 0);
    sbi(PORTC, 0);

    cbi(DDRC, 5);
    cbi(PORTC, 5);


    ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS0);
  	ADMUX = 0b00000101;

    ADCSRA |= 1<<ADSC;		// Start Conversion

    display.SetDigits(-1);
}

static volatile uint16_t ADCReading;

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


void loop()
{    
    /*
        - Measure value
        - Find nearest in table.
            - If value > table end
                - Switch to higher sampler
            - If value < table end
                - Switch to lower sampler
        - Display value from table        
    */
    
    uint16_t sampledValue;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        sampledValue = ADCReading;
    }

    uint16_t displayValue;
    if(sampledValue <= pgm_read_word_near(E24ADCValues_200Ohm)) {
        displayValue = 0;
    } else if (sampledValue >= pgm_read_word_near(E24ADCValues_200Ohm + E24ValuesCount - 1)) {
        displayValue = 999;
    } else {
        const uint16_t *nearestElement = 
            (const uint16_t *)bsearch(
                &sampledValue,
                E24ADCValues_200Ohm, 
                E24ValuesCount, 
                sizeof(E24ADCValues_200Ohm[0]),
                [](const void *key, const void *element) {
                    if(element <= E24ADCValues_200Ohm) {
                        //display.SetSymbol(LCDDisplay::LEDSymbol::CautionTriangle);
                        return -1;
                    } else if (element >= E24ADCValues_200Ohm + E24ValuesCount - 1) {
                       // display.SetSymbol(LCDDisplay::LEDSymbol::CautionTriangle);
                        return 1;
                    } else {
                        
                        

                        const uint16_t sampledValue = *((uint16_t *)key);
                        const uint16_t prevValue = pgm_read_word_near((uint16_t *)element - 1);
                        const uint16_t value = pgm_read_word_near(element);
                        const uint16_t nextValue = pgm_read_word_near((uint16_t *)element + 1);
                        
                        /*display.SetDigits((uint16_t *)element - E24ADCValues_200Ohm);
                        delay(500);
                        display.SetDigits(prevValue);
                        delay(500);
                        display.SetDigits(value);
                        delay(500);
                        display.SetDigits(nextValue);
                        delay(500);
                        display.SetDigits(sampledValue);
                        delay(1000);*/
                        
                        if(sampledValue <= value - (value - prevValue) / 2) {
                            return -1;
                        } else if(sampledValue > value + (nextValue - value) / 2) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
            }); 
            
        int foundIndex = nearestElement - E24ADCValues_200Ohm;
        displayValue = pgm_read_word_near(E24Values + foundIndex);
        //display.SetDigits(foundIndex);
        //delay(2000);
    }

    //display.SetDigits(sampledValue);
    display.SetDigits(displayValue);
    //delay(2000);
                    
/*    
    static uint16_t currentDisplay = 0;

    uint16_t newDisplay;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        newDisplay = ADCReading;
    }

    if(newDisplay != currentDisplay) {
        display.SetDigits(newDisplay);
        currentDisplay = newDisplay;

        display.ToggleSymbol(LCDDisplay::LEDSymbol::CautionTriangle);
        
        for(uint8_t i = 0; i < E24ValuesCount; ++i) {
            uint16_t read = pgm_read_word_near(E24Values + i);
            display.SetDigits(read);
        }
    }*/
}


ISR(ADC_vect)
{
    uint16_t thisReading = ADC;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ADCReading = thisReading;
    }

	ADCSRA |= 1<<ADSC;		// Start Conversion
}
