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

// Want to map symbolic name to array of ports and segments.
// Also want to map digits and positions to array of ports and segments.

// First, define symbolic names.
class LEDSymbol {
public:
    enum Name {
        DottedUpsidedownT,
        ChartGoingNegative,
        BLANK,
        DownArrowOnLeft,
        SevenSeg1E,
        MinusLarge,
        SevenSeg1A,
        SevenSeg1F,
        SevenSeg1C,
        SevenSeg1D,
        SevenSeg1B,
        SevenSeg1G,
        SevenSeg2E,
        Colon,
        SevenSeg2A,
        SevenSeg2F,
        SevenSeg2C,
        SevenSeg2D,
        SevenSeg2B,
        SevenSeg2G,
        SevenSeg3E,
        DecimalPoint,
        SevenSeg3A,
        SevenSeg3F,
        SevenSeg3C,
        SevenSeg3D,
        SevenSeg3B,
        SevenSeg3G,
        FireCrossed,
        CautionTriangle,
        MinusLeft,
        DownArrowOnRight,
        Percent,
        Celcius,
        MilliAmp,
        Bar,
        UpArrowOnRight,
        Plus,
        Blower,
        Fire,
        Tap,
        Two,
        Radiator,
        TriangleInCircle,
        Sun,
        Target,
        One,
        UpArrowOnLeft,
    };

    LEDSymbol() = default; 
    constexpr LEDSymbol(Name name) : _name(name) {}

    constexpr uint8_t CommonPin() const { return (uint8_t)(_name % 4); };
    constexpr uint16_t SegmentPin() const { return (uint8_t)(_name / 4); };

private:
    Name _name;
};

static const LEDSymbol DigitSegmentSymbols[3][7] = {
    {
        LEDSymbol::SevenSeg3A,
        LEDSymbol::SevenSeg3B,
        LEDSymbol::SevenSeg3C,
        LEDSymbol::SevenSeg3D,
        LEDSymbol::SevenSeg3E,
        LEDSymbol::SevenSeg3F,
        LEDSymbol::SevenSeg3G,
    },
    {
        LEDSymbol::SevenSeg2A,
        LEDSymbol::SevenSeg2B,
        LEDSymbol::SevenSeg2C,
        LEDSymbol::SevenSeg2D,
        LEDSymbol::SevenSeg2E,
        LEDSymbol::SevenSeg2F,
        LEDSymbol::SevenSeg2G,
    },
    {
        LEDSymbol::SevenSeg1A,
        LEDSymbol::SevenSeg1B,
        LEDSymbol::SevenSeg1C,
        LEDSymbol::SevenSeg1D,
        LEDSymbol::SevenSeg1E,
        LEDSymbol::SevenSeg1F,
        LEDSymbol::SevenSeg1G,
    }
};

static const uint16_t DigitSegmentPortMaskForCommon[4] {
      (DigitSegmentSymbols[0][0].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[0][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][1].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[0][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][2].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[0][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][3].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[0][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][4].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[0][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][5].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[0][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][6].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[0][6].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][0].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[1][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][1].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[1][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][2].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[1][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][3].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[1][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][4].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[1][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][5].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[1][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][6].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[1][6].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][0].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[2][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][1].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[2][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][2].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[2][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][3].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[2][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][4].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[2][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][5].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[2][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][6].CommonPin() == 0 ? (uint16_t)_BV(DigitSegmentSymbols[2][6].SegmentPin()) : 0),
    
      (DigitSegmentSymbols[0][0].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[0][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][1].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[0][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][2].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[0][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][3].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[0][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][4].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[0][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][5].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[0][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][6].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[0][6].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][0].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[1][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][1].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[1][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][2].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[1][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][3].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[1][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][4].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[1][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][5].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[1][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][6].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[1][6].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][0].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[2][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][1].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[2][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][2].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[2][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][3].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[2][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][4].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[2][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][5].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[2][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][6].CommonPin() == 1 ? (uint16_t)_BV(DigitSegmentSymbols[2][6].SegmentPin()) : 0),

      (DigitSegmentSymbols[0][0].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[0][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][1].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[0][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][2].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[0][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][3].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[0][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][4].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[0][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][5].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[0][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][6].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[0][6].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][0].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[1][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][1].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[1][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][2].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[1][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][3].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[1][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][4].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[1][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][5].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[1][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][6].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[1][6].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][0].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[2][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][1].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[2][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][2].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[2][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][3].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[2][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][4].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[2][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][5].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[2][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][6].CommonPin() == 2 ? (uint16_t)_BV(DigitSegmentSymbols[2][6].SegmentPin()) : 0),

      (DigitSegmentSymbols[0][0].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[0][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][1].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[0][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][2].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[0][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][3].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[0][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][4].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[0][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][5].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[0][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[0][6].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[0][6].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][0].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[1][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][1].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[1][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][2].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[1][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][3].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[1][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][4].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[1][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][5].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[1][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[1][6].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[1][6].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][0].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[2][0].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][1].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[2][1].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][2].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[2][2].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][3].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[2][3].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][4].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[2][4].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][5].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[2][5].SegmentPin()) : 0)
    | (DigitSegmentSymbols[2][6].CommonPin() == 3 ? (uint16_t)_BV(DigitSegmentSymbols[2][6].SegmentPin()) : 0)
};

static const uint8_t SevenSegmentCodes[10] = { 
    0b0111111, 
    0b0000110, 
    0b1011011, 
    0b1001111, 
    0b1100110, 
    0b1101101, 
    0b1111101, 
    0b0000111, 
    0b1111111,
    0b1101111 
};

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

static uint16_t displaySegmentStateByCommon[4] = {0};

// uint16_terrupt service routine 
#ifdef __AVR_ATmega8__
ISR(TIMER2_COMP_vect)
#else
ISR(TIMER2_COMPA_vect)        // uint16_terrupt service routine 
#endif
{
    static uint8_t phase = 0;
    
#if 1
    // From DataWeek "Bare LCD display drive in embedded applications"
    const uint8_t common = (phase % 4);
    const bool commonUpPhase = (phase < 4);
#else
    // From AVR340 Application Note. 
    const uint8_t common = (phase / 2);
    const bool commonUpPhase = (phase % 2);
#endif

    // We'll copy this from global storage once per cycle - see below.
    static uint16_t segmentPinStateByCommon[4] = {0};

    uint16_t segmentPinsToLight = segmentPinStateByCommon[common];

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
        for(auto port : segmentPorts) {
            if((segmentPinsToLight & 0b1)) {
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
            segmentPinsToLight >>= 1;
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
    
    phase = phase + 1;
    if(phase == 8) {
        phase = 0;

        // Copy this once per cycle, instead of reading it directly,
        // so that eacy entire cyce has a 'balanced' AC voltage for
        // each segment.
        // Not sure if this is actually necessary?
        // Do it at the end of the interrupt so that the above stuff
        // runs at 'more constant' time w.r.t. the timer firing.
        memcpy(segmentPinStateByCommon, displaySegmentStateByCommon, sizeof(displaySegmentStateByCommon));
    }

    interrupts();
}

void SetDigits(uint16_t digits) {
    uint16_t segmentPinsToLight[4] = {0};

    if(digits >= 1000) {
        digits = 999;
    }

    {
        auto digit = digits % 10;
        uint8_t segmentBitPattern = SevenSegmentCodes[digit];
        for(int i = 0; segmentBitPattern != 0; ++i) {
            if(segmentBitPattern & 0b1) {
                LEDSymbol symbol = DigitSegmentSymbols[0][i];
                segmentPinsToLight[symbol.CommonPin()] |= _BV(symbol.SegmentPin());
            }
            segmentBitPattern >>= 1;
        }
    }

    if(digits >= 10) {
        auto digit = (digits / 10) % 10;
        uint8_t segmentBitPattern = SevenSegmentCodes[digit];
        for(int i = 0; segmentBitPattern != 0; ++i) {
            if(segmentBitPattern & 0b1) {
                LEDSymbol symbol = DigitSegmentSymbols[1][i];
                segmentPinsToLight[symbol.CommonPin()] |= _BV(symbol.SegmentPin());
            }
            segmentBitPattern >>= 1;
        }
    }

    if(digits >= 100) {
        auto digit = (digits / 100) % 10;
        uint8_t segmentBitPattern = SevenSegmentCodes[digit];
        for(int i = 0; segmentBitPattern != 0; ++i) {
            if(segmentBitPattern & 0b1) {
                LEDSymbol symbol = DigitSegmentSymbols[2][i];
                segmentPinsToLight[symbol.CommonPin()] |= _BV(symbol.SegmentPin());
            }
            segmentBitPattern >>= 1;
        }
    }

    noInterrupts();

    for(int i = 0; i < 4; ++i) {
        displaySegmentStateByCommon[i] = (displaySegmentStateByCommon[i] & ~(DigitSegmentPortMaskForCommon[i])) | segmentPinsToLight[i];
    }

    interrupts();
}

void loop()
{    
    auto timeNow = millis();

    static typeof(timeNow) halfSeconds = 0;
    typeof(timeNow) halfSecondsNow = (timeNow / 500);
    if(halfSecondsNow != halfSeconds) {
        halfSeconds = halfSecondsNow;
        if(halfSecondsNow % 2) {
            SetDigits(halfSecondsNow / 2);
        }

        LEDSymbol symbol = LEDSymbol::CautionTriangle;
        uint16_t bit = _BV(symbol.SegmentPin());
        if((displaySegmentStateByCommon[symbol.CommonPin()] & bit) == bit) { 
            displaySegmentStateByCommon[symbol.CommonPin()] &= (~bit);
        } else {
            displaySegmentStateByCommon[symbol.CommonPin()] |= bit;
        }
    }
}

