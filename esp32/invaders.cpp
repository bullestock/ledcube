#include <Arduino.h>
#include "program.hpp"
#include "display.hpp"
#include "block.h"
#include "random8.h"

class Invaders : public Program
{
public:
    static const int INVADERS_DOWN_COUNT = 5;
    
    Invaders()
        : Program(4),
          defender(pixelFromHex(LightBlue),
                   3, 3, 0,
                   3, 3, 1)
    {
        defender.show();

        invaders[0].b.set_position(Position(invaders_x + 1, 1, invaders_z));
        invaders[1].b.set_position(Position(invaders_x + 4, 1, invaders_z));
        invaders[2].b.set_position(Position(invaders_x + 1, 4, invaders_z));
        invaders[3].b.set_position(Position(invaders_x + 4, 4, invaders_z));
        invaders[4].b.set_position(Position(invaders_x + 1, 7, invaders_z));
        invaders[5].b.set_position(Position(invaders_x + 4, 7, invaders_z));
        for (auto& i : invaders)
        {
            i.b.set_size(Position(2, 2, 2));
            i.b.set_colour(pixelFromHex(HotPink));
            i.b.show();
        }
    }

    virtual bool run()
    {
        if (limiter.skip()) return false;

        if (game_over)
        {
            clear_all();
            if (++game_over_frame > NUM_LEDS_PER_ROW)
            {
                game_over_frame = 0;
                game_over = false;
                return true;
            }
            Block b(pixelFromHex(LimeGreen),
                    NUM_LEDS_PER_ROW/2, NUM_LEDS_PER_ROW/2, NUM_LEDS_PER_ROW/2,
                    game_over_frame, game_over_frame, game_over_frame);
            b.show();
            return true;
        }
        
        if (move_defender)
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

            // Every fourth time: Move invaders
            if (move_invaders)
            {
                if (invaders_down <= 0)
                {
                    invaders_down = INVADERS_DOWN_COUNT;
                    for (auto& i : invaders)
                        if (i.alive)
                            i.b.move(DIR_DOWN, 1);
                    --invaders_z;
                    if (invaders_z <= 1)
                        game_over = true;
                }
                else
                {
                    for (auto& i : invaders)
                        if (i.alive)
                            i.b.move(invaders_left ? DIR_LEFT : DIR_RIGHT, 1);

                    invaders_x += invaders_left ? -1 : 1;
                    if (invaders_x < 1)
                    {
                        invaders_left = false;
                        --invaders_down;
                    }
                    if (invaders_x > 2)
                    {
                        invaders_left = true;
                        --invaders_down;
                    }
                }
            }
            move_invaders = !move_invaders;
        }
        move_defender = !move_defender;
        
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
    bool game_over = false;
    int game_over_frame = 0;
    Block defender;
    struct Invader
    {
        Block b;
        bool alive = true;
    };
    Invader invaders[6];
    int invaders_x = 0;
    int invaders_z = 7;
    bool invaders_left = false;
    int invaders_down = INVADERS_DOWN_COUNT;
    Position fire_pos;
    bool firing = false;
    pixelColor_t fire_colour = pixelFromHex(FireBrick);
    bool move_defender = false;
    bool move_invaders = false;
};

REGISTER_PROGRAM(Invaders);
