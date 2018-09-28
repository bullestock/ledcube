#include <Arduino.h>
#include "program.hpp"
#include "display.hpp"
#include "chasecolours.hpp"
#include "random8.h"

class Snake : public Program
{
public:
    Snake()
        : Program(2)
    {
    }

    static const int NOF_SNAKES = 1;

    enum Direction {
        DIR_UP,
        DIR_LEFT,
        DIR_FRONT,
        DIR_RIGHT,
        DIR_BACK,
        DIR_DOWN,
        DIR_SIZE
    };
    
    bool run() override;

private:
    struct SnakeData
    {
        bool active = false;
        int x = 0;
        int y = 0;
        int z = 0;
        Direction dir = DIR_UP;
        pixelColor_t c;
    };

    bool limit_hit_imminent(Direction dir, int x, int y, int z)
    {
        advance(dir, x, y, z);
        return
           (x < 0) ||
           (x >= NUM_LEDS_PER_ROW) ||
           (y < 0) ||
           (y >= NUM_LEDS_PER_ROW) ||
           (z < 0) ||
           (z >= NUM_LEDS_PER_ROW);
    }

    void advance(Direction dir, int& x, int& y, int& z)
    {
        switch (dir)
        {
        case DIR_UP:
            ++z;
            break;
        case DIR_LEFT:
            --x;
            break;
        case DIR_FRONT:
            ++y;
            break;
        case DIR_RIGHT:
            ++x;
            break;
        case DIR_BACK:
            --y;
            break;
        case DIR_DOWN:
            --z;
            break;
        default:
            internal_error();
            break;
        }
    }

    SnakeData snakes[NOF_SNAKES];
    ChaseColours cc;
};

bool Snake::run()
{
    if (limiter.skip()) return false;

    const int idx = 0;
    auto& s = snakes[idx];

    if (!s.active)
    {
        // Choose random start position and direction
        s.x = random8(0, NUM_LEDS_PER_ROW);
        s.y = random8(0, NUM_LEDS_PER_ROW);
        s.z = random8(0, NUM_LEDS_PER_ROW);
        s.dir = (Direction) random8(0, DIR_SIZE);
        s.c = cc.get();
        cc.next();
        set_pixel(s.x, s.y, s.z, s.c);
        s.active = true;
    }
    else
    {
        // Update
        while (limit_hit_imminent(s.dir, s.x, s.y, s.z))
            // Choose new random direction
            s.dir = (Direction) random8(0, DIR_SIZE);
        advance(s.dir, s.x, s.y, s.z);
        set_pixel(s.x, s.y, s.z, s.c);
    }
    fade_all(10);
    return true;
}

REGISTER_PROGRAM(Snake);