#include <Arduino.h> 

#include "LCDDisplay.h"

static LCDDisplay display;

void setup()
{
    display.SetUp();  
}

void loop()
{    
    auto timeNow = millis();

    static typeof(timeNow) halfSeconds = 0;
    typeof(timeNow) halfSecondsNow = (timeNow / 500);
    if(halfSecondsNow != halfSeconds) {
        halfSeconds = halfSecondsNow;
        if(halfSecondsNow % 2) {
            display.SetDigits(halfSecondsNow / 2);
        }

        display.ToggleSymbol(LCDDisplay::LEDSymbol::CautionTriangle);
    }
}

