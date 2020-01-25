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

#endif