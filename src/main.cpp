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
    const byte portNumber;
    const byte offset;
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
    { 2, 5 },
    { 2, 4 },
    { 2, 3 },
    { 2, 2 },
};

static const PortDescriptor segmentPorts[12] = {
    { 2, 6 },
    { 2, 7 },
    { 0, 0 },
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 0, 4 },
    { 1, 0 },
    { 1, 1 },
    { 1, 2 },
    { 1, 3 },
    { 1, 4 },
};

static const int LCD_AC_FREQ = 32;

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
    
    static const int timer_reset_count = ((F_CPU / 1024) / ((int)LCD_AC_FREQ * (int)8));
    static_assert(timer_reset_count <= 255, "timer_reset_count is too large!");
    
    
    noInterrupts(); 
    
    #ifdef __AVR_ATmega8__
    {
        TCCR2 = 0;
        TCCR2 |= (1 << WGM21);              	           // CTC ("Clear Timer/Counter") Mode
        TCCR2 |= (1 << CS22) | (1 << CS21) | (1 << CS20);  // A 1024 prescaler  
        
        TIMSK |= (1 << OCIE2);                            // Output Compare Interupt Enable
        
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

// interrupt service routine 
#ifdef __AVR_ATmega8__
ISR(TIMER2_COMP_vect)
#else
ISR(TIMER2_COMPA_vect)        // interrupt service routine 
#endif
{
    static byte counter = 0;
    static byte segmentTestPhase = 0;
    static byte commonTestPhase = 0;
    
#if 1
    // From DataWeek "Bare LCD display drive in embedded applications"
    const byte phase = (counter % 8);
    const byte common = (phase % 4);
    const bool commonUpPhase = (phase < 4);
#else
    // From AVR340 Application Note. 
    const byte phase = (counter % 8);
    const byte common = (phase / 2);
    const bool commonUpPhase = (phase % 2);
#endif

    noInterrupts(); 

    byte buildPorts[] = { *Ports[0], *Ports[1], *Ports[2] };
    byte buildDDRs[] = { *DDRs[0], *DDRs[1], *DDRs[2] };
        
    {
        byte i = 0;
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
        byte i = 0;
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
        
    for(int i = 0; i < 3; ++i) {
        if(*DDRs[i] != buildDDRs[i]) {
            *DDRs[i] = buildDDRs[i];
        }
        if(*Ports[i] != buildPorts[i]) {
            *Ports[i] = buildPorts[i];
        }
    }
    
    interrupts();
   
    ++counter;
    
    if((counter % (LCD_AC_FREQ * 2)) == 0) {
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

