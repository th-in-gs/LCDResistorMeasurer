//
//  ADCTables.c
//  
//
//  Created by James Montgomerie on 12/27/19.
//

#include "ADCTables.h"
#include "math.h"

template<uint8_t group, uint8_t offset>
static constexpr uint16_t EValue()
{
    uint16_t ret = 0;
    if(group < 48) {
        ret = (uint16_t)(round(pow(10.0, (double)(offset)/(double)group) * 10.0));
        switch(ret) {
            case 26:
            case 29:
            case 32:
            case 35:
            case 38:
            case 42:
            case 46:
                ret += 1;
                break;
            case 83:
                ret -= 1;
                break;
            default:
                break;
        }
        ret *= 10;
    } else {
        ret = (uint16_t)(round(pow(10.0, (double)(offset)/(double)group) * 100.0));
    }
    return ret * 10;
}

const uint16_t E24Values[] PROGMEM = {
    EValue<24, 23>() / 10,
    EValue<24, 0>(),
    EValue<24, 1>(),
    EValue<24, 2>(),
    EValue<24, 3>(),
    EValue<24, 4>(),
    EValue<24, 5>(),
    EValue<24, 6>(),
    EValue<24, 7>(),
    EValue<24, 8>(),
    EValue<24, 9>(),
    EValue<24, 10>(),
    EValue<24, 11>(),
    EValue<24, 12>(),
    EValue<24, 13>(),
    EValue<24, 14>(),
    EValue<24, 15>(),
    EValue<24, 16>(),
    EValue<24, 17>(),
    EValue<24, 18>(),
    EValue<24, 19>(),
    EValue<24, 20>(),
    EValue<24, 21>(),
    EValue<24, 22>(),
    EValue<24, 23>(),
    EValue<24, 0>() * 10,
};
const uint8_t E24ValuesCount = sizeof(E24Values)/sizeof(E24Values[0]);

constexpr uint16_t E96Values[] PROGMEM = {
    EValue<96, 95>() / 10,
    EValue<96, 0>(),
    EValue<96, 1>(),
    EValue<96, 2>(),
    EValue<96, 3>(),
    EValue<96, 4>(),
    EValue<96, 5>(),
    EValue<96, 6>(),
    EValue<96, 7>(),
    EValue<96, 8>(),
    EValue<96, 9>(),
    EValue<96, 10>(),
    EValue<96, 11>(),
    EValue<96, 12>(),
    EValue<96, 13>(),
    EValue<96, 14>(),
    EValue<96, 15>(),
    EValue<96, 16>(),
    EValue<96, 17>(),
    EValue<96, 18>(),
    EValue<96, 19>(),
    EValue<96, 20>(),
    EValue<96, 21>(),
    EValue<96, 22>(),
    EValue<96, 23>(),
    EValue<96, 24>(),
    EValue<96, 25>(),
    EValue<96, 26>(),
    EValue<96, 27>(),
    EValue<96, 28>(),
    EValue<96, 29>(),
    EValue<96, 30>(),
    EValue<96, 31>(),
    EValue<96, 32>(),
    EValue<96, 33>(),
    EValue<96, 34>(),
    EValue<96, 35>(),
    EValue<96, 36>(),
    EValue<96, 37>(),
    EValue<96, 38>(),
    EValue<96, 39>(),
    EValue<96, 40>(),
    EValue<96, 41>(),
    EValue<96, 42>(),
    EValue<96, 43>(),
    EValue<96, 44>(),
    EValue<96, 45>(),
    EValue<96, 46>(),
    EValue<96, 47>(),
    EValue<96, 48>(),
    EValue<96, 49>(),
    EValue<96, 50>(),
    EValue<96, 51>(),
    EValue<96, 52>(),
    EValue<96, 53>(),
    EValue<96, 54>(),
    EValue<96, 55>(),
    EValue<96, 56>(),
    EValue<96, 57>(),
    EValue<96, 58>(),
    EValue<96, 59>(),
    EValue<96, 60>(),
    EValue<96, 61>(),
    EValue<96, 62>(),
    EValue<96, 63>(),
    EValue<96, 64>(),
    EValue<96, 65>(),
    EValue<96, 66>(),
    EValue<96, 67>(),
    EValue<96, 68>(),
    EValue<96, 69>(),
    EValue<96, 70>(),
    EValue<96, 71>(),
    EValue<96, 72>(),
    EValue<96, 73>(),
    EValue<96, 74>(),
    EValue<96, 75>(),
    EValue<96, 76>(),
    EValue<96, 77>(),
    EValue<96, 78>(),
    EValue<96, 79>(),
    EValue<96, 80>(),
    EValue<96, 81>(),
    EValue<96, 82>(),
    EValue<96, 83>(),
    EValue<96, 84>(),
    EValue<96, 85>(),
    EValue<96, 86>(),
    EValue<96, 87>(),
    EValue<96, 88>(),
    EValue<96, 89>(),
    EValue<96, 90>(),
    EValue<96, 91>(),
    EValue<96, 92>(),
    EValue<96, 93>(),
    EValue<96, 94>(),
    EValue<96, 95>(),
    EValue<96, 0>() * 10,   
};
const uint8_t E96ValuesCount = sizeof(E96Values)/sizeof(E96Values[0]);

static_assert(E24ValuesCount == 26, "E24ValuesCount should be 26");
static_assert(E96ValuesCount == 98, "E96ValuesCount should be 98");
