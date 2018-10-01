#include <Arduino.h>
#include "program.hpp"
#include "display.hpp"
#include "block.h"
#include "random8.h"

class Invaders : public Program
{
public:
    Invaders()
        : Program(2),
          defender(pixelFromHex(LightBlue),
                   3, 3, 0,
                   3, 3, 1)
    {
        defender.show();
    }

    virtual bool run()
    {
        if (limiter.skip()) return false;

        while (1)
        {
            const auto dir = random_direction();
            if ((dir == DIR_UP) || (dir == DIR_DOWN))
                continue;
            const auto amount = random8(1, 3);
            if (defender.move(dir, amount))
                break;
        }
        
        return true;
    }

private:
    Block defender;
};

REGISTER_PROGRAM(Invaders);
