#include <Arduino.h>
#include "program.hpp"
#include "display.hpp"

class GameOfLife : public Program
{
public:
    GameOfLife()
        : Program(10)
    {
    }

    bool run() override;

private:
    static int neighbors(const Position& pos)
    {
        int count = 0;

        for (int x = pos.x-1; x <= pos.x+1; x++)
            for (int y = pos.y-1; y <= pos.y+1; y++)
                for (int z = pos.z-1; z <= pos.z+1; z++)
                {
                    if (!(z < 0 ||
                          z >= NUM_LEDS_PER_ROW ||
                          y < 0 ||
                          y >= NUM_LEDS_PER_ROW ||
                          x < 0 ||
                          x >= NUM_LEDS_PER_ROW ||
                          (x == z && y == y && z == z)))
                    {
                        if (!is_black(Position(x, y, z)))
                            ++count;
                    }
                }
        return count;
    }
    
    int coloured_pixels = 0;
    uint8_t wheel_pos = 0;
    bool fading_in = true;
    int cur_x = 0;
    int cur_y = 0;
    int cur_z = 0;
};

//REGISTER_PROGRAM(GameOfLife);

bool GameOfLife::run()
{
    if (limiter.skip()) return false;

    if (fading_in)
    {
        for (int i = 0; i < 10; ++i)
        {
            const auto p = random_position();
            if (is_black(p))
            {
                ++coloured_pixels;
                set_pixel(p, get_wheel_colour(wheel_pos++));
            }
            if (coloured_pixels >= 200)
            {
                fading_in = false;
                cur_x = 0;
                cur_y = 0;
                cur_z = 0;
                limiter.setFps(0.2);
            }
        }
        return true;
    }

    const bool do_all = true;
    
    bool done = false;
    if (do_all)
        cur_x = cur_y = cur_z = 0;
    do
    {
        const Position pos(cur_x, cur_y, cur_z);
        const auto neighbor_count = neighbors(pos);
        if (is_black(pos))
        {
            if (neighbor_count == 4)
            {
                set_pixel(pos, get_wheel_colour(wheel_pos + cur_x*2 + cur_z*3 + cur_y));
                ++coloured_pixels;
                done = !do_all;
            }
        }
        else
        {
            if (!(neighbor_count == 2 || neighbor_count == 3))
            {
                set_pixel(pos, pixelFromRGB(0, 0, 0));
                --coloured_pixels;
                done = !do_all;
            }
        }
        ++cur_x;
        if (cur_x >= NUM_LEDS_PER_ROW)
        {
            ++cur_y;
            cur_x = 0;
        }
        if (cur_y >= NUM_LEDS_PER_ROW)
        {
            ++cur_z;
            cur_y = 0;
        }
        if (cur_z >= NUM_LEDS_PER_ROW)
        {
            cur_z = 0;
            if (do_all)
                break;
        }
    } while (!done);
    
    return true;
}
