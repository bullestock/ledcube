#include <Arduino.h>
#include "display.hpp"
#include "esp32_digital_led_lib.h"

void set_pixel(int idx, pixelColor_t c)
{
    const int s = idx/NUM_LEDS_PER_STRAND;
    if (s > NUM_OF_STRANDS)
        return;
    const int l = idx % NUM_LEDS_PER_STRAND;
    pixels[s][l] = c;
}

void set_pixel(int x, int y, int z, pixelColor_t c)
{
    if ((x < 0) ||
        (x >= NUM_LEDS_PER_ROW) ||
        (y < 0) ||
        (y >= NUM_LEDS_PER_ROW) ||
        (z < 0) ||
        (z >= NUM_LEDS_PER_ROW))
    {
        Serial.print("Invalid coordinates: ");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.print(", ");
        Serial.println(z);
        internal_error();
        return;
    }
    // y = strand
    pixels[y][z * NUM_OF_STRANDS + x] = c;
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
            set_pixel(i, pixelFromRGB(255, 0, 0));
        show();
        delay(1000);
        clear_all();
        delay(1000);
    }
}
