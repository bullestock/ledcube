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

    static const int NOF_SNAKES = 5;

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
        int age = 0;
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

    void change_direction(Direction& dir)
    {
        while (1)
        {
            const auto new_dir = (Direction) random8(0, DIR_SIZE);
            bool ok = false;
            switch (dir)
            {
            case DIR_UP:
                ok = (new_dir != DIR_DOWN);
                break;
            case DIR_LEFT:
                ok = (new_dir != DIR_RIGHT);
                break;
            case DIR_FRONT:
                ok = (new_dir != DIR_BACK);
                break;
            case DIR_RIGHT:
                ok = (new_dir != DIR_RIGHT);
                break;
            case DIR_BACK:
                ok = (new_dir != DIR_FRONT);
                break;
            case DIR_DOWN:
                ok = (new_dir != DIR_UP);
                break;
            default:
                internal_error();
                break;
            }
            if (ok)
            {
                dir = new_dir;
                return;
            }
        }
    }

    SnakeData snakes[NOF_SNAKES];
    ChaseColours cc;
};

bool Snake::run()
{
    if (limiter.skip()) return false;

    for (auto& s : snakes)
    {
        // Randomly kill old snakes
        if (s.age > 50 + random8(0, 100))
            s.age = 0;
        if (s.age == 0)
        {
            // Initialize: Choose random start position and direction
            s.x = random8(0, NUM_LEDS_PER_ROW);
            s.y = random8(0, NUM_LEDS_PER_ROW);
            s.z = random8(0, NUM_LEDS_PER_ROW);
            s.dir = (Direction) random8(0, DIR_SIZE);
            s.c = cc.get();
            cc.next();
            set_pixel(s.x, s.y, s.z, s.c);
        }
        else
        {
            // Update
            if (random8(0, 10) < 4)
                change_direction(s.dir);
            
            while (limit_hit_imminent(s.dir, s.x, s.y, s.z))
                change_direction(s.dir);

            advance(s.dir, s.x, s.y, s.z);
            set_pixel(s.x, s.y, s.z, s.c);
        }
        ++s.age;
    }
    fade_all(10);
    return true;
}

REGISTER_PROGRAM(Snake);
