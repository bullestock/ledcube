/*
 * Neomatrix - Copyright (C) 2017 Lars Christensen
 * MIT Licensed
 *
 * Main display code
 */

#include "esp32_digital_led_lib.h"
#include <esp_timer.h>
#include <WiFi.h>

#include "common.hpp"
#include "display.hpp"
#include "neomatrix.hpp"
#include "program.hpp"

Program* current = nullptr;
uint32_t startTime = 0;
ProgramFactory* currentFactory = nullptr;

extern void show();

void neomatrix_init()
{
    currentFactory = ProgramFactory::first;
    current = currentFactory->launch();
    char buf[80];
    sprintf(buf, "Launched %s", currentFactory->name);
    Serial.println(buf);
    startTime = millis();
}

#define RUNTIME 60000
#define FADETIME 3000
#define FADEIN FADETIME
#define FADEOUT (RUNTIME-FADETIME)

static int max_brightness = BRIGHTNESS;
static bool night_mode = false;

bool run_autonomously = true;
bool auto_program_switch = true;

void set_brightness(int brightness)
{
}

void program_loop()
{
    uint32_t now = millis();
    uint32_t prgTime = now - startTime;
    if (prgTime < FADEIN)
    {
        // set_brightness((prgTime * max_brightness) / FADEIN);
    }
    else if (prgTime < FADEOUT)
    {
        // auto brightness = max_brightness;
        // if (night_mode)
        //     brightness = std::min(brightness, 64);
        // set_brightness(brightness);
    }
    else if (prgTime < RUNTIME)
    {
        // set_brightness(((RUNTIME - prgTime) * max_brightness) / FADETIME);
    }
    else
    {
        clear_all();
        show();
        delete current;

        do
        {
            neomatrix_next_program();
        }
        while (night_mode && !current->allow_night_mode());

        startTime = now;
    }
}

void neomatrix_run()
{
    if (auto_program_switch)
        program_loop();
    if (run_autonomously)
    {
        if (current->run())
            show();
    }
}

void neomatrix_change_program(const char* name)
{
    auto p = ProgramFactory::get(name);
    if (!p)
    {
        Serial.println("Not found");
        return;
    }
    current = p->launch();
    auto_program_switch = false;
    clear_all();
    set_brightness(max_brightness);
}

void neomatrix_next_program()
{
    currentFactory = currentFactory->next;
    if (!currentFactory)
        currentFactory = ProgramFactory::first;
    current = currentFactory->launch();
    Serial.print("Next: ");
    Serial.println(currentFactory->name);
 }

void neomatrix_set_speed(int fps)
{
    if (current)
        current->limiter.setFps(fps);
}

void neomatrix_set_brightness(uint8_t brightness)
{
    max_brightness = brightness;
    set_brightness(max_brightness);
}

void neomatrix_set_nightmode(bool nightmode)
{
    night_mode = nightmode;
}

void neomatrix_start_autorun()
{
    run_autonomously = true;
    auto_program_switch = true;
}

pixelColor_t get_pixel(int idx)
{
    const int s = idx/NUM_LEDS_PER_STRAND;
    if (s > NUM_OF_STRANDS)
        return pixelColor_t();
    const int l = idx % NUM_LEDS_PER_STRAND;
    return pixels[s][l];
}


void set_pixel(int idx, pixelColor_t c)
{
    const int s = idx/NUM_LEDS_PER_STRAND;
    if (s > NUM_OF_STRANDS)
        return;
    const int l = idx % NUM_LEDS_PER_STRAND;
    pixels[s][l] = c;
}

