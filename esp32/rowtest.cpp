#include <Arduino.h>

#include "program.hpp"
#include "display.hpp"

// Single wandering dot
class RowTest : public Program
{
public:
    RowTest()
        : Program(2)
    {
    }

    virtual bool run()
    {
        if (limiter.skip()) return false;

        if (idx > 0)
            set_pixel(idx-1, pixelFromRGB(0, 0, 0));
        if (idx >= NUM_LEDS)
            idx = 0;
        set_pixel(idx, pixelFromRGB(0, 255, 0));
        ++idx;
        return true;
    }

private:
    int idx = 0;
};

REGISTER_PROGRAM(RowTest);
