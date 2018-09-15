#include <Arduino.h>
#include "program.hpp"
#include "display.hpp"

// The Matrix-style scrolling display
class Matrix : public Program
{
public:
    Matrix()
        : Program(2)
    {
    }

    virtual bool run()
    {
        if (limiter.skip()) return false;

        // Shift up
        for (int s = 0; s < NUM_OF_STRANDS; ++s)
        {
            const int offset = s * NUM_LEDS_PER_STRAND;
            for (int r = NUM_ROWS_PER_STRAND - 1; r > 0; --r)
                for (int i = 0; i < NUM_LEDS_PER_ROW; ++i)
                    set_pixel(offset + r * NUM_LEDS_PER_ROW + i, get_pixel(offset + (r - 1) * NUM_LEDS_PER_ROW + i));
        }
        // Add random bits at bottom rows
        for (int s = 0; s < NUM_OF_STRANDS; ++s)
        {
            const int offset = s * NUM_LEDS_PER_STRAND;
            for (int i = 0; i < NUM_LEDS_PER_ROW; ++i)
            {
                const int rand = random(0, 100);
                if (rand > 90)
                    set_pixel(offset + i, pixelFromRGB(0, 255, 0));
                else
                    set_pixel(offset + i, pixelFromRGB(0, 0, 0));
            }
        }
        return true;
    }
};

REGISTER_PROGRAM(Matrix);
