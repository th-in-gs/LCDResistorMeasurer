/**
* Techniques from DataWeek "Bare LCD display drive in embedded applications"
* http://www.dataweek.co.za/article.aspx?pklArticleId=2382&pklCategoryId=31
*/

#include <Arduino.h> 
#include <avr/sleep.h> 


#ifndef cbi
#define cbi(sfr, bit) (sfr &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (sfr |= _BV(bit))
#endif 
#ifndef tbi
#define tbi(sfr, bit) ((sfr&_BV(bit))?cbi(sfr, bit):sbi(sfr, bit))
#endif 


struct PortDescriptor {
    const uint8_t portNumber;
    const uint8_t offset;
};

static const typeof(&PORTB) Ports[] = {
    &PORTB,
    &PORTC,
    &PORTD,
};

static const typeof(&DDRB) DDRs[] = {
    &DDRB,
    &DDRC,
    &DDRD,
};

static const PortDescriptor commonPorts[4] = {
    { 2, 0 }, // LCD Pin  1
    { 2, 1 }, // LCD Pin  2
    { 2, 2 }, // LCD Pin 17
    { 2, 3 }, // LCD Pin 18
};

static const PortDescriptor segmentPorts[12] = {
    { 2, 4 }, // LCD Pin  3
    { 0, 6 }, // LCD Pin  4
    { 0, 7 }, // LCD Pin  5
    { 2, 5 }, // LCD Pin  6
    { 2, 6 }, // LCD Pin  7
    { 2, 7 }, // LCD Pin  8
    { 0, 0 }, // LCD Pin  9
    { 0, 1 }, // LCD Pin 10
    { 0, 2 }, // LCD Pin 11
    { 0, 3 }, // LCD Pin 12
    { 0, 4 }, // LCD Pin 13
    { 0, 5 }, // LCD Pin 14
};

static const uint8_t SevenSegmentCodes[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

static const uint16_t LCDPinToBitPosition[19] = {
    0,       // N/A
    _BV(11), // Pin  1
    _BV(12), // Pin  2
    _BV( 0), // Pin  3
    _BV( 1), // Pin  4
    _BV( 2), // Pin  5
    _BV( 3), // Pin  6
    _BV( 4), // Pin  7
    _BV( 5), // Pin  8
    _BV( 6), // Pin  9
    _BV( 7), // Pin 10
    _BV( 8), // Pin 11
    _BV( 9), // Pin 12
    _BV(10), // Pin 13
    _BV(11), // Pin 14
    0,       // Pin 15
    0,       // Pin 16
    _BV(13), // Pin 17
    _BV(14), // Pin 18
};


struct CommonAndSegment {
    uint8_t commonPin;
    uint8_t segmentPin;
};

static const CommonAndSegment CommonAndSegmentNULL = { 0, 0 };

static const 

/*
static uint16_t numberPattern(uint8_t digit, uint8_t position) {
    for (uint8_t i = 0, bits = SevenSegmentCodes[digit]; bits > 0; bits >>= 1, ++i) {
        if(bits & 0b1) {

        }
        ++i;
    }
}
*/

static const uint8_t LCD_AC_FREQ = 32;

void setup()
{	
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    
    for(auto port : commonPorts) {
        sbi(*DDRs[port.portNumber], port.offset);
        cbi(*Ports[port.portNumber], port.offset);
    }
    for(auto port : segmentPorts) {
        sbi(*DDRs[port.portNumber], port.offset);
        cbi(*Ports[port.portNumber], port.offset);
    }
    
    sbi(DDRB, 5);
    cbi(PORTB, 5);
    
    static const uint16_t timer_reset_count = ((F_CPU / 1024) / ((uint16_t)LCD_AC_FREQ * (uint16_t)8));
    static_assert(timer_reset_count <= 255, "timer_reset_count is too large!");
    
    
    noInterrupts(); 
    
    #ifdef __AVR_ATmega8__
    {
        TCCR2 = 0;
        TCCR2 |= (1 << WGM21);              	           // CTC ("Clear Timer/Counter") Mode
        TCCR2 |= (1 << CS22) | (1 << CS21) | (1 << CS20);  // A 1024 prescaler  
        
        TIMSK |= (1 << OCIE2);                             // Output Compare Interupt Enable
        
        // Set up the counter output compare to get the desired frequency.
        // Check for overflow, just in case...
        OCR2 = timer_reset_count;	
        TCNT2 = 0;
    }
    #else
    {
        TCCR2A = (1 << WGM21);              	            // CTC ("Clear Timer/Counter") Mode
        TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);  // A 1024 prescaler  
        
        TIMSK2 = (1 << OCIE2A);                            // Output Compare Interupt Enable
        
        // Set up the counter output compare to get the desired frequency.
        // Check for overflow, just in case...
        OCR2A = timer_reset_count;	
        TCNT2 = 0;
    }
    #endif
    
    interrupts();
}

// uint16_terrupt service routine 
#ifdef __AVR_ATmega8__
ISR(TIMER2_COMP_vect)
#else
ISR(TIMER2_COMPA_vect)        // uint16_terrupt service routine 
#endif
{
    static uint8_t counter = 0;
    static uint8_t segmentTestPhase = 0;
    static uint8_t commonTestPhase = 0;
    
#if 1
    // From DataWeek "Bare LCD display drive in embedded applications"
    const uint8_t phase = (counter % 8);
    const uint8_t common = (phase % 4);
    const bool commonUpPhase = (phase < 4);
#else
    // From AVR340 Application Note. 
    const uint8_t phase = (counter % 8);
    const uint8_t common = (phase / 2);
    const bool commonUpPhase = (phase % 2);
#endif

    noInterrupts(); 

    uint8_t buildPorts[] = { *Ports[0], *Ports[1], *Ports[2] };
    uint8_t buildDDRs[] = { *DDRs[0], *DDRs[1], *DDRs[2] };
        
    {
        uint8_t i = 0;
        for(auto port : commonPorts) {
            if(i == common) {
                sbi(buildDDRs[port.portNumber], port.offset);
                if(commonUpPhase) {
                    sbi(buildPorts[port.portNumber], port.offset);
                } else {
                    cbi(buildPorts[port.portNumber], port.offset);
                }
            } else {
                cbi(buildDDRs[port.portNumber], port.offset);	
                cbi(buildPorts[port.portNumber], port.offset);
            }
            ++i;
        }
    }

    {
        uint8_t i = 0;
        for(auto port : segmentPorts) {
            if(i == segmentTestPhase && common == commonTestPhase) {
                if(commonUpPhase) {
                    cbi(buildPorts[port.portNumber], port.offset);
                } else {
                    sbi(buildPorts[port.portNumber], port.offset);
                }
            } else {
                if(commonUpPhase) {
                    sbi(buildPorts[port.portNumber], port.offset);
                } else {
                    cbi(buildPorts[port.portNumber], port.offset);
                }
            }
            ++i;
        }
    }
        
    for(uint16_t i = 0; i < 3; ++i) {
        if(*DDRs[i] != buildDDRs[i]) {
            *DDRs[i] = buildDDRs[i];
        }
        if(*Ports[i] != buildPorts[i]) {
            *Ports[i] = buildPorts[i];
        }
    }
    
    interrupts();
   
    ++counter;
    
    static const uint8_t resetSteps = []() {
        uint8_t i = (LCD_AC_FREQ * 2);
        i = (i + 7) / 8 * 8;
        return i;
    }();
    
    if((counter % resetSteps) == 0) {
        counter = 0;
        if(++segmentTestPhase == 12) {
            segmentTestPhase = 0;
            if(++commonTestPhase == 4) {
                commonTestPhase = 0;
            }
        }
    }
}

void loop()
{
    
	    
}

