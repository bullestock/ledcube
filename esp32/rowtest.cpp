#include "program.hpp"
#include "display.hpp"

// Single wandering dot
class RowTest : public Program
{
public:
    RowTest()
        : Program(5)
    {
    }

    virtual bool run()
    {
        if (limiter.skip()) return false;

        if (idx > 0)
            leds[idx-1] = CRGB(0, 0, 0);
        leds[idx] = CRGB(0, 255, 0);
        ++idx;
        return true;
    }

private:
    int idx = 0;
};

REGISTER_PROGRAM(RowTest);
