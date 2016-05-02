/**
* Techniques from http://www.dataweek.co.za/article.aspx?pklArticleId=2382&pklCategoryId=31
*/

#include <Arduino.h> 

#ifndef cbi
#define cbi(sfr, bit) (sfr &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (sfr |= _BV(bit))
#endif 

struct PortDescriptor {
	typeof(PORTB) * const port;
	typeof(DDRB) * const ddr; 
	const byte offset;
};

static const PortDescriptor commonPorts[4] = {
	{ &PORTB, &DDRB, 0 },
	{ &PORTD, &DDRD, 7 },
	{ &PORTD, &DDRD, 6 },
	{ &PORTD, &DDRD, 5 },
};

static const PortDescriptor segmentPorts[12] = {
	{ &PORTB, &DDRB, 1 },
	{ &PORTB, &DDRB, 2 },
	{ &PORTB, &DDRB, 3 },
	{ &PORTB, &DDRB, 4 },
	{ &PORTC, &DDRC, 0 },
	{ &PORTC, &DDRC, 1 },
	{ &PORTC, &DDRC, 2 },
	{ &PORTC, &DDRC, 3 },
	{ &PORTC, &DDRC, 4 },
	{ &PORTC, &DDRC, 5 },
	{ &PORTD, &DDRD, 4 },
	{ &PORTD, &DDRD, 3 },
};

static const int LCD_AC_FREQ = 31;

void setup()
{	
	for(auto port : commonPorts) {
		sbi(*port.ddr, port.offset);
		sbi(*port.port, port.offset);
	}
	for(auto port : segmentPorts) {
		sbi(*port.ddr, port.offset);
		sbi(*port.port, port.offset);
	}

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
	static byte counter = 0;
	static byte segmentTestPhase = 0;
	static byte segmentItemTestPhase = 0;

	const byte phase = (counter % 8);
	const byte common = (phase % 4);
	const bool commonUpPhase = (phase < 4);
	
	{
		byte i = 0;
		for(auto port : commonPorts) {
			if(i == common) {
				sbi(*port.ddr, port.offset);
				if(commonUpPhase) {
					sbi(*port.port, port.offset);
				} else {
					cbi(*port.port, port.offset);
				}
			} else {
				cbi(*port.ddr, port.offset);	
				sbi(*port.port, port.offset);
			}
			++i;
		}
	}
	
	{
		byte i = 0;
		for(auto port : segmentPorts) {
			if(i == segmentTestPhase &&
			   common == segmentItemTestPhase) {
				if(commonUpPhase) {
					cbi(*port.port, port.offset);
				} else {
					sbi(*port.port, port.offset);
				}
			} else {
				if(commonUpPhase) {
					sbi(*port.port, port.offset);
				} else {
					cbi(*port.port, port.offset);
				}
			}
			++i;
		}
	}
	
	if(++counter == 64) {
		counter = 0;
		if(++segmentTestPhase == 12) {
			segmentTestPhase = 0;
			if(++segmentItemTestPhase == 4) {
				segmentItemTestPhase = 0;
			}
		}
	}
}

void loop()
{
}
