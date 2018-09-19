// Copyright (C) 2017-2019 Torsten Martinsen <torsten@bullestock.net>

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "esp32_digital_led_lib.h"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <Wire.h>

#include "display.hpp"
#include "neomatrix.hpp"

const char* version = "0.0.1";

const char* ssids[] = {
    "bullestock-guest",
    "hal9k"
};
const char* password = "";

MDNSResponder mdns;
const char myDNSName[] = "rgbcube";

WiFiUDP Udp;

// Pins for controlling strands
constexpr int PixelPins[] = {
    13, 12, 14, 27, 26, 25, 33, 32
};
const int ButtonPin = 22;
const int DEBOUNCE_TIME_MS = 100;

void show();

strand_t strands[NUM_OF_STRANDS];
pixelColor_t* pixels[NUM_OF_STRANDS];

void setup()
{
    delay(1000);
    Serial.begin(115200);
    Serial.print("\r\nRGB LED Cube ");
    Serial.println(version);

    pinMode(ButtonPin, INPUT_PULLUP);

    for (int i = 0; i < NUM_OF_STRANDS; ++i)
    {
        strands[i].rmtChannel = i;
        strands[i].gpioNum = PixelPins[i];
        strands[i].ledType = LED_WS2812B_V3;
        strands[i].brightLimit = BRIGHTNESS;
        strands[i].numPixels = NUM_LEDS_PER_STRAND;
        strands[i].pixels = nullptr;
        strands[i]._stateVars = nullptr;
        if (digitalLeds_initStrands(&strands[i], 1))
        {
            Serial.println("Strand init failure");
            while (1)
                ;
        }
        pixels[i] = strands[i].pixels;
    }
    
#if 0
    
    // Connect to WiFi network
    WiFi.mode(WIFI_STA);
    int index = 0;
    while (1)
    {
        Serial.println();
        Serial.println();
        Serial.print("Trying to connect to ");
        Serial.println(ssids[index]);

        WiFi.disconnect();
        WiFi.begin(ssids[index], password);

        int i = 0;
        bool connected = false;
        while (i < 15)
        {
            delay(250);
            if (WiFi.status() == WL_CONNECTED)
            {
                connected = true;
                break;
            }
            delay(250);
            Serial.print(".");
            ++i;
        }
        if (connected)
        {
            Serial.println("");
            Serial.print("Connected to ");
            Serial.println(ssids[index]);
            delay(1000);
            break;
        }
        Serial.println("");
        ++index;
        if (index >= sizeof(ssids)/sizeof(const char*))
            index = 0;
    }
  
    // Print the IP address
    Serial.println(WiFi.localIP());

    // Set up mDNS responder:
    if (!mdns.begin(myDNSName))
    {
        Serial.println("Error setting up mDNS responder!");
    }
    else
    {
        Serial.println("mDNS responder started");
        Serial.printf("My name is [%s]\r\n", myDNSName);
        delay(1000);
    }

    Udp.begin(7890);

#endif

    clear_all();
    show();
    neomatrix_init();
}

WiFiClient client;

bool dirtyshow = false;

extern bool run_autonomously;

void parse_pixel_data(uint8_t* data, int size)
{
    if (size < sizeof(int16_t))
        return;

    if (run_autonomously)
    {
        run_autonomously = false;
        Serial.println("Switch to non-autonomous mode");
    }
    
    auto cmdptr = (int16_t*) data;
    size -= sizeof(int16_t);
    auto pixrgb = data + sizeof(int16_t);
    auto nof_leds = size/3;
    int offset = *cmdptr;
    //Serial.printf("Offset: %i, Size: %i\n", offset, size / 3);
    for (int i = offset; i < std::min(offset + nof_leds, NUM_LEDS); ++i)
        set_pixel(i, pixelFromRGB(*pixrgb++, *pixrgb++, *pixrgb++));
    dirtyshow = true;
}

void parse_mode(uint8_t* data, int size)
{
    if (size < sizeof(uint8_t))
        return;
    data[size] = 0;
    auto mode = (const char*) data;
    Serial.printf("Set mode %s\r\n", mode);
    neomatrix_change_program(mode);
}

void parse_speed(uint8_t* data, int size)
{
    if (size < sizeof(uint8_t))
        return;
    auto autonomous_speed = *data;
    if (autonomous_speed > 50)
        autonomous_speed = 50;
    if (autonomous_speed <= 0)
        autonomous_speed = 1;
    Serial.printf("Set speed %d\r\n", autonomous_speed);
    neomatrix_set_speed(autonomous_speed);
}

void parse_brightness(uint8_t* data, int size)
{
    if (size < sizeof(uint8_t))
        return;
    auto brightness = *data;
    if (brightness < 10)
        brightness = 10;
    Serial.printf("Set brightness %d\r\n", brightness);
    neomatrix_set_brightness(brightness);
}

void parse_nightmode(uint8_t* data, int size)
{
    if (size < sizeof(uint8_t))
        return;
    auto nightmode = *data;
    if (nightmode > 1)
        nightmode = 1;
    Serial.printf("Set night mode %d\r\n", nightmode);
    neomatrix_set_nightmode(nightmode);
}

void clientEventUdp()
{
    int32_t packetSize = 0;
    while ((packetSize = Udp.parsePacket()))
    {
        uint8_t rcv[10 + NUM_LEDS * 3];
        auto len = Udp.read(rcv, sizeof(rcv));
        auto cmdptr = (uint16_t*) rcv;
        auto cmd = cmdptr[0];
        switch (cmd)
        {
        case 1111:
            // Pixel data
            parse_pixel_data(rcv + sizeof(int16_t), len - sizeof(int16_t));
            break;

        case 1112:
            // Set autonomous mode (string)
            parse_mode(rcv + sizeof(int16_t), len - sizeof(int16_t));
            break;
            
        case 1114:
            // Set autonomous speed (1 byte)
            parse_speed(rcv + sizeof(int16_t), len - sizeof(int16_t));
            break;
            
        case 1115:
            // Start autonomous mode switch (no arguments)
            neomatrix_start_autorun();
            break;

        case 1116:
            // Set brightness (1 byte)
            parse_brightness(rcv + sizeof(int16_t), len - sizeof(int16_t));
            break;

        case 1117:
            // Night mode (1 byte)
            parse_nightmode(rcv + sizeof(int16_t), len - sizeof(int16_t));
            break;
            
        default:
            break;
        }
    }
}

void clear_all()
{
    for (int i = 0; i < NUM_OF_STRANDS; ++i)
        memset(pixels[i], 0, NUM_LEDS_PER_STRAND * sizeof(pixelColor_t));
}

void show()
{
    for (int i = 0; i < NUM_OF_STRANDS; ++i)
    {
        Serial.print("num "); Serial.println(strands[i].numPixels); delay(100);
        digitalLeds_updatePixels(&strands[i]);
        Serial.println("upd done"); delay(100);
    }
}

int p = 0;
void loop()
{
    static bool last_pressed = false;
    static bool button_state = false;
    static unsigned long last_debounce = 0;

    clientEventUdp();

    neomatrix_run();
    delay(10);

    if (dirtyshow)
    {
        show();
        dirtyshow = false;
    }

    const bool pressed = !digitalRead(ButtonPin);
    if (pressed != last_pressed)
        last_debounce = millis();
    if (millis() - last_debounce > DEBOUNCE_TIME_MS)
        if (pressed != button_state)
        {
            button_state = pressed;
            Serial.print("Button: ");
            Serial.println(button_state);
            if (button_state)
                neomatrix_next_program();
        }
    last_pressed = pressed;
}
