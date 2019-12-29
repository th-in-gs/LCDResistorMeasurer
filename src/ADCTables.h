//
//  ADCTables.h
//  
//
//  Created by James Montgomerie on 12/27/19.
//

#ifndef ADCTables_h
#define ADCTables_h

#include <avr/pgmspace.h>
#include <stdint.h>

extern const uint16_t E24Values[] PROGMEM;
extern const uint16_t E24ADCValues_200Ohm[] PROGMEM;

extern const uint8_t E24ValuesCount;

#endif /* ADCTables_h */
