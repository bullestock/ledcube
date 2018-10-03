#include <Arduino.h>
#include "display.hpp"
#include "esp32_digital_led_lib.h"
#include "random8.h"

bool is_inside(const Position& pos)
{
    return (pos.x >= 0) && (pos.x < NUM_LEDS_PER_ROW) &&
        (pos.y >= 0) && (pos.y < NUM_LEDS_PER_ROW) &&
       (pos.z >= 0) && (pos.z < NUM_LEDS_PER_ROW);
}

void set_pixel(int idx, pixelColor_t c)
{
    const int s = idx/NUM_LEDS_PER_STRAND;
    if (s > NUM_OF_STRANDS)
        return;
    const int l = idx % NUM_LEDS_PER_STRAND;
    pixels[s][l] = c;
}


pixelColor_t get_pixel(int idx)
{
    const int s = idx/NUM_LEDS_PER_STRAND;
    if (s > NUM_OF_STRANDS)
        return pixelColor_t();
    const int l = idx % NUM_LEDS_PER_STRAND;
    return pixels[s][l];
}

void check_position(const Position& pos)
{
    if ((pos.x < 0) ||
        (pos.x >= NUM_LEDS_PER_ROW) ||
        (pos.y < 0) ||
        (pos.y >= NUM_LEDS_PER_ROW) ||
        (pos.z < 0) ||
        (pos.z >= NUM_LEDS_PER_ROW))
    {
        Serial.print("Invalid coordinates: ");
        Serial.print(pos.x);
        Serial.print(", ");
        Serial.print(pos.y);
        Serial.print(", ");
        Serial.println(pos.z);
        internal_error();
    }
}

void set_pixel(const Position& pos, pixelColor_t c)
{
    check_position(pos);
    // y = strand
    pixels[pos.y][pos.z * NUM_LEDS_PER_ROW + NUM_LEDS_PER_ROW - pos.x - 1] = c;
}

void set_pixel(int x, int y, int z, pixelColor_t c)
{
    set_pixel(Position(x, y, z), c);
}

pixelColor_t get_pixel(const Position& pos)
{
    check_position(pos);
    return pixels[pos.y][pos.z * NUM_OF_STRANDS + pos.x];
}

bool is_black(const Position& pos)
{
    check_position(pos);
    return pixels[pos.y][pos.z * NUM_OF_STRANDS + pos.x] == pixelFromRGB(0, 0, 0);
}

void clear_all()
{
    for (int i = 0; i < NUM_OF_STRANDS; ++i)
        memset(pixels[i], 0, NUM_LEDS_PER_STRAND * sizeof(pixelColor_t));
}

void fade_all(int percent)
{
    const double factor = 1.0 - percent/100.0;
    for (int i = 0; i < NUM_OF_STRANDS; ++i)
        for (int j = 0; j < NUM_LEDS_PER_STRAND; ++j)
        {
            pixels[i][j].r *= factor;
            pixels[i][j].g *= factor;
            pixels[i][j].b *= factor;
        }
}

void internal_error()
{
    while (1)
    {
        for (int i = 0; i < NUM_LEDS; ++i)
            set_pixel(i, pixelFromRGB(32, 0, 0));
        show();
        delay(1000);
        clear_all();
        show();
        delay(1000);
    }
}

void change_direction(Direction& dir)
{
    Serial.print("change_direction: ");
    Serial.println(dir);
    int tries = 0;
    while (1)
    {
        const auto new_dir = (Direction) random8(0, DIR_SIZE);
        Serial.print("trying ");
        Serial.println(new_dir);
        if (!is_opposite(dir, new_dir))
        {
            dir = new_dir;
            Serial.println("accepted");
            return;
        }
        if (++tries > 20)
        {
            Serial.print("Cannot find opposite of ");
            Serial.println(dir);
            internal_error();
        }
    }
}

bool is_opposite(Direction d1, Direction d2)
{
    switch (d1)
    {
    case DIR_UP:
        return (d2 == DIR_DOWN);

    case DIR_LEFT:
        return (d2 == DIR_RIGHT);

    case DIR_FRONT:
        return (d2 == DIR_BACK);

    case DIR_RIGHT:
        return (d2 == DIR_RIGHT);

    case DIR_BACK:
        return (d2 == DIR_FRONT);

    case DIR_DOWN:
        return (d2 == DIR_UP);

    default:
        internal_error();
        break;
    }
    return false;
}

Position translate(const Position& pos, Direction dir, int amount)
{
    Position new_pos = pos;
    int dx = 0, dy = 0, dz = 0;
    switch (dir)
    {
    case DIR_UP:
        dz = 1;
        break;

    case DIR_LEFT:
        dx = -1;
        break;

    case DIR_FRONT:
        dy = 1;
        break;

    case DIR_RIGHT:
        dx = 1;
        break;

    case DIR_BACK:
        dy = -1;
        break;

    case DIR_DOWN:
        dz = -1;
        break;

    default:
        internal_error();
        break;
    }
    new_pos.x += dx;
    new_pos.y += dy;
    new_pos.z += dz;
    return new_pos;
}

void scroll(Direction dir)
{
    // TODO
}

Position random_position()
{
    return Position(random8(0, NUM_LEDS_PER_ROW),
                    random8(0, NUM_LEDS_PER_ROW),
                    random8(0, NUM_LEDS_PER_ROW));
}

Direction random_direction()
{
    return (Direction) random8(0, DIR_SIZE);
}

pixelColor_t get_wheel_colour(uint8_t pos)
{
    pos = 255 - pos;
    if (pos < 85)
        return pixelFromRGB(255 - pos * 3, 0, pos * 3);
    if (pos < 170)
    {
        pos -= 85;
        return pixelFromRGB(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return pixelFromRGB(pos * 3, 255 - pos * 3, 0);
}
