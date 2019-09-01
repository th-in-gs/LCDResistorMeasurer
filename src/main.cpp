#include <Arduino.h> 

#include "LCDDisplay.h"

#include <util/atomic.h>

static LCDDisplay display;

void setup()
{
    display.SetUp();  

    sbi(DDRC, 0);
    sbi(PORTC, 0);

    cbi(DDRC, 5);
    cbi(PORTC, 5);


    ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS0) ;
  	ADMUX = 0b00000101;

    ADCSRA |= 1<<ADSC;		// Start Conversion

    display.SetDigits(0);

    delay(1000);
}

static volatile uint16_t ADCReading;

void loop()
{    
    static uint16_t currentDisplay = 0;

    uint16_t newDisplay;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        newDisplay = ADCReading / 4;
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

    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        ADCReading = thisReading;
    }

	ADCSRA |= 1<<ADSC;		// Start Conversion
}
