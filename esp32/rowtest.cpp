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

        clear_all();
        set_pixel(x, y, z, pixelFromRGB(0, 255, 0));
        ++x;
        if (x >= NUM_LEDS_PER_ROW)
        {
            ++y;
            x = 0;
        }
        if (y >= NUM_LEDS_PER_ROW)
        {
            ++z;
            y = 0;
        }
        if (z >= NUM_LEDS_PER_ROW)
        {
            z = 0;
        }
        return true;
    }

private:
    int x = 0;
    int y = 0;
    int z = 0;
};

//REGISTER_PROGRAM(RowTest);
