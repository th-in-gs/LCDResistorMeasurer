/**
* Techniques from http://www.dataweek.co.za/article.aspx?pklArticleId=2382&pklCategoryId=31
*/

#include "Arduino.h"
#include <avr/pgmspace.h> 

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 

enum TriState {
	off = false,
	on = true,
	nc,
};

static const TriState commons[8][4] = { 
	{ on,  nc,  nc,  nc  },
	{ nc,  on,  nc,  nc  },
	{ nc,  nc,  on,  nc  },
	{ nc,  nc,  nc,  on  },
	{ off, nc,  nc,  nc  },
	{ nc,  off, nc,  nc  },
	{ nc,  nc,  off, nc  },
	{ nc,  nc,  nc,  off },
};

static const struct { const typeof(PORTB) *port; const typeof(DDRB) *ddr; const byte offset;} commonPorts[4] = {
	{ &PORTB, &DDRB, 0 },
	{ &PORTD, &DDRD, 7 },
	{ &PORTD, &DDRD, 6 },
	{ &PORTD, &DDRD, 5 },
};

static const struct { const typeof(PORTB) *port; const byte offset; } segmentPorts[12] = {
	{ &PORTB, 1 },
	{ &PORTB, 2 },
	{ &PORTB, 3 },
	{ &PORTB, 4 },
	{ &PORTC, 0 },
	{ &PORTC, 1 },
	{ &PORTC, 2 },
	{ &PORTC, 3 },
	{ &PORTC, 4 },
	{ &PORTC, 5 },
	{ &PORTD, 4 },
	{ &PORTD, 3 },
};

static const int LCD_AC_FREQ = 31;

void setup()
{	
	noInterrupts(); 

	static const int timer_reset_count = ((F_CPU / 1024) / (LCD_AC_FREQ * 8));
	static_assert(timer_reset_count <= 255, "timer_reset_count is too large!");
	
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
		TCCR2A = (1 << WGM21);              	           // CTC ("Clear Timer/Counter") Mode
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
	
	static byte phase = 0;
	phase = (phase + 1)	% 8;
	
	{
		byte i = 0;
		for(auto state : commons[phase]) {
			auto port = commonPorts[i];
			switch(state) {
				case on:
					sbi(*port.ddr, port.offset);
					sbi(*port.port, port.offset);
					break;
				case off:	
					sbi(*port.ddr, port.offset);
					cbi(*port.port, port.offset);
					break;
				case nc:
					cbi(*port.ddr, port.offset);
					cbi(*port.port, port.offset);
					break;
			}
			++i;
		}
	}
	
	{	
		static byte segmentTestPhase = 0;
		if(phase == 0) {
			segmentTestPhase = (segmentTestPhase + 1) % 12;
		}

		byte i = 0;
		for(auto port : segmentPorts) {
			if(i == segmentTestPhase) {
				if(phase < 4) {
					cbi(*port.port, port.offset);
				} else {
					sbi(*port.port, port.offset);
				}
			} else {
				if(phase < 4) {
					sbi(*port.port, port.offset);
				} else {
					cbi(*port.port, port.offset);
				}
			}
			++i;
		}
	}
}

void loop()
{
}
