#include <Arduino.h>
#include "program.hpp"
#include "display.hpp"
#include "block.h"
#include "random8.h"

class Invaders : public Program
{
public:
    
    Invaders()
        : Program(4),
          defender(pixelFromHex(LightBlue),
                   3, 3, 0,
                   3, 3, 1)
    {
        defender.show();
    }

    virtual bool run()
    {
        if (limiter.skip()) return false;

        if (move)
        {
            // Every second time: Move defender
        
            while (1)
            {
                const auto dir = random_direction();
                if ((dir == DIR_UP) || (dir == DIR_DOWN))
                    continue;
                const auto amount = random8(1, 3);
                if (defender.move(dir, amount))
                    break;
            }

            // Fire at random

            if (!firing && (random8(0, 10) < 3))
            {
                fire_pos = defender.position();
                ++fire_pos.z;
                firing = true;
            }
        }
        move = !move;
        
        if (firing)
        {
            if (fire_pos.z < NUM_LEDS_PER_ROW)
                set_pixel(fire_pos, fire_colour);
            if (fire_pos.z > 1)
            {
                auto old_pos = fire_pos;
                --old_pos.z;
                set_pixel(old_pos, pixelFromRGB(0, 0, 0));
            }
            if (++fire_pos.z > NUM_LEDS_PER_ROW)
                firing = false;
        }
        
        return true;
    }

private:
    Block defender;
    Position fire_pos;
    bool firing = false;
    pixelColor_t fire_colour = pixelFromHex(FireBrick);
    bool move = false;
};

REGISTER_PROGRAM(Invaders);
