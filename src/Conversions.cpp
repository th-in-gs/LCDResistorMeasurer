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


static uint8_t i32toa_inner(int32_t value, char *string) {
    uint8_t length = 0;
    if(value >= 10) {
        length = i32toa_inner(value / 10, string);
    }
    string[length] = '0' + value % 10;
    return length + 1;
}

uint8_t i32toa(int32_t value, char *string) {
    uint8_t length = i32toa_inner(value, string);
    string[length] = '\0';
    return length;
}