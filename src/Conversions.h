//
//  Conversions.h
//  
//
//  Created by James Montgomerie on 01/19/20.
//


#ifndef Conversions_h
#define Conversions_h

#include <stdint.h>

uint32_t ADCToOhms(uint16_t reading, uint32_t referenceOhms);

uint8_t i32toa(int32_t value, char *string);

#endif