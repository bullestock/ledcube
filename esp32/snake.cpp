#include <Arduino.h>
#include "program.hpp"
#include "display.hpp"
#include "chasecolours.hpp"
#include "random8.h"

class Snake : public Program
{
public:
    Snake()
        : Program(5)
    {
    }

    static const int NOF_SNAKES = 5;

    bool run() override;

private:
    struct SnakeData
    {
        int age = 0;
        Position pos;
        Direction dir = DIR_UP;
        pixelColor_t c;
    };

    bool limit_hit_imminent(Direction dir, const Position& pos)
    {
        auto new_pos = pos;
        advance(dir, new_pos);
        if ((new_pos.x < 0) ||
            (new_pos.x >= NUM_LEDS_PER_ROW) ||
            (new_pos.y < 0) ||
            (new_pos.y >= NUM_LEDS_PER_ROW) ||
            (new_pos.z < 0) ||
            (new_pos.z >= NUM_LEDS_PER_ROW))
            return true;
        const auto p = get_pixel(new_pos);
        //Serial.println(p.r + p.g + p.b);
        return p.r + p.g + p.b > 50;
    }

    void advance(Direction dir, Position& pos)
    {
        switch (dir)
        {
        case DIR_UP:
            ++pos.z;
            break;
        case DIR_LEFT:
            --pos.x;
            break;
        case DIR_FRONT:
            ++pos.y;
            break;
        case DIR_RIGHT:
            ++pos.x;
            break;
        case DIR_BACK:
            --pos.y;
            break;
        case DIR_DOWN:
            --pos.z;
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

    int idx = 0;
    for (auto& s : snakes)
    {
        ++idx;
        // Randomly kill old snakes
        if (s.age > 50 + random8(0, 100))
            s.age = 0;
        if (s.age == 0)
        {
            // Initialize: Choose random start position and direction
            s.pos = random_position();
            s.dir = random_direction();
            s.c = cc.get();
            cc.next();
            set_pixel(s.pos, s.c);
        }
        else
        {
            // Update
            if (random8(0, 10) < 4)
                change_direction(s.dir);

            int tries = 0;
            const auto old_dir = s.dir;
            while (limit_hit_imminent(s.dir, s.pos))
            {
                if (++tries > 10)
                {
                    Serial.print("Give up on ");
                    Serial.println(idx);
                    s.age = 0;
                    fade_all(20);
                    return true;
                }
                int dir_tries = 0;
                do
                {
                    change_direction(s.dir);
                    if (++dir_tries > 20)
                    {
                        Serial.print("Cannot find opposite of ");
                        Serial.print(old_dir);
                        Serial.print(" at ");
                        Serial.print(s.pos.x);
                        Serial.print(", ");
                        Serial.print(s.pos.y);
                        Serial.print(", ");
                        Serial.println(s.pos.z);
                        internal_error();
                    }
                }
                while (is_opposite(s.dir, old_dir));
            }
            if (s.age > 0)
            {
                advance(s.dir, s.pos);
                set_pixel(s.pos, s.c);
            }
        }
        ++s.age;
    }
    fade_all(20);
    return true;
}

REGISTER_PROGRAM(Snake);
