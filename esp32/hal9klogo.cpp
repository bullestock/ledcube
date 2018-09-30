#include <Arduino.h>
#include "program.hpp"
#include "display.hpp"
#include "text.h"

class Hal9kLogo : public Program
{
public:
    Hal9kLogo()
        : Program(8),
          text("HAL9K")
    {
    }

    // Show the letters H A L 9 K in sequence
    // Each letter starts at the back and scrolls to the front
    virtual bool run()
    {
        if (limiter.skip()) return false;

        text.show();
        return true;
    }

private:
    Text text;
    //Axis axis = AXIS_Y;
};

REGISTER_PROGRAM(Hal9kLogo);
