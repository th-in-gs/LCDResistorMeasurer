//
//  Conversions.cpp
//  
//
//  Created by James Montgomerie on 01/19/20.
//

#include "Conversions.h"

uint32_t ADCToOhms(uint16_t reading, uint32_t referenceOhms) {
/*
  - - --+-- - -
        |
        +-----  1024
        |
       +-+
       | |
       | |      reference Ω
       | |
       +-+
        |
        +-----  reading
        |
       +-+
       | |
       | |      unknown Ω
       | |
       +-+
        |    
        +-----  0
        |
       ---
        -    

    (reference + unknown) / 1024 == reference / (1024 - reading)

             reference + unknown == 1024 * (reference / (1024 - reading))
                         
                         unknown == 1024 * (reference / (1024 - reading)) - reference
                              
                                 == (reference * 1024) / (1024 - reading) - reference

*/

    if(reading == 0) {
        return UINT32_MAX;
    } else if(reading >= 1023) {
        return 0;
    } else {
        uint32_t reading32 = reading;
        uint32_t reference32 = referenceOhms;
                        
        return ((reference32 * 1024) / (1024 - reading32)) - reference32;
    } 
}