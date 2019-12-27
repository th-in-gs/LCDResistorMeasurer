#include <Arduino.h> 
#include <util/atomic.h>

#include "LCDDisplay.h"

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
    static uint16_t currentDisplay = 0;

    uint16_t newDisplay;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        newDisplay = ADCReading;
    }

    if(newDisplay != currentDisplay) {
        display.SetDigits(newDisplay);
        currentDisplay = newDisplay;

        display.ToggleSymbol(LCDDisplay::LEDSymbol::CautionTriangle);
    }
}

ISR(ADC_vect)
{
    uint16_t thisReading = ADCL;
    thisReading |= ADCH << 8;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ADCReading = thisReading;
    }

	ADCSRA |= 1<<ADSC;		// Start Conversion
}
