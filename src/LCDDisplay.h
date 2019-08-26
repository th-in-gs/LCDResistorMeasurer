#ifndef LCDDisplay_h
#define LCDDisplay_h

#include <stdint.h> 

class LCDDisplay {
public:

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

    LCDDisplay() = default;

    void SetUp();

    void SetDigits(const uint16_t digits);

    void SetSymbol(const LEDSymbol symbol);
    void ClearSymbol(const LEDSymbol symbol);
    void ToggleSymbol(const LEDSymbol symbol);
};

#endif