//
//  ADCTables.h
//  
//
//  Created by James Montgomerie on 12/27/19.
//

#ifndef ADCTables_h
#define ADCTables_h

#if __has_include(<avr/pgmspace.h>) 
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#include <stdint.h>

extern const uint16_t E24Values[] PROGMEM;
extern const uint8_t  E24ValuesCount;

extern const uint16_t E96Values[] PROGMEM;
extern const uint8_t E96ValuesCount;

#endif /* ADCTables_h */
