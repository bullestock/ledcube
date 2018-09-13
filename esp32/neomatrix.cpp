/*
 * Neomatrix - Copyright (C) 2017 Lars Christensen
 * MIT Licensed
 *
 * Main display code
 */

#include <FastLED.h>
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

void program_loop()
{
    uint32_t now = millis();
    uint32_t prgTime = now - startTime;
    if (prgTime < FADEIN)
    {
        FastLED.setBrightness((prgTime * max_brightness) / FADEIN);
    }
    else if (prgTime < FADEOUT)
    {
        auto brightness = max_brightness;
        if (night_mode)
            brightness = std::min(brightness, 64);
        FastLED.setBrightness(brightness);
    }
    else if (prgTime < RUNTIME)
    {
        FastLED.setBrightness(((RUNTIME - prgTime) * max_brightness) / FADETIME);
    }
    else
    {
        FastLED.setBrightness(0);
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
    FastLED.setBrightness(max_brightness);
}

void neomatrix_next_program()
{
    currentFactory = currentFactory->next;
    if (!currentFactory)
        currentFactory = ProgramFactory::first;
    current = currentFactory->launch();
    Serial.print("Launched ");
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
    FastLED.setBrightness(max_brightness);
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
