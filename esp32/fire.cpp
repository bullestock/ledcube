#include "program.hpp"
#include "display.hpp"

#define LIB8STATIC __attribute__ ((unused)) static inline
#define LIB8STATIC_ALWAYS_INLINE __attribute__ ((always_inline)) static inline
typedef uint8_t   fract8;   ///< ANSI: unsigned short _Fract
typedef uint16_t  fract16;  ///< ANSI: unsigned       _Fract

#define RAND16_SEED  1337
uint16_t rand16seed = RAND16_SEED;

#include "math8.h"
#include "random8.h"

// Approximates a 'black body radiation' spectrum for
// a given 'heat' level.  This is useful for animations of 'fire'.
// Heat is specified as an arbitrary scale from 0 (cool) to 255 (hot).
// This is NOT a chromatically correct 'black body radiation'
// spectrum, but it's surprisingly close, and it's fast and small.

pixelColor_t HeatColor( uint8_t temperature)
{
    // Scale 'heat' down from 0-255 to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    uint8_t t192 = scale8_video( temperature, 191);

    // calculate a value that ramps up from
    // zero to 255 in each 'third' of the scale.
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252

    pixelColor_t heatcolor;
    heatcolor.w = 0;
    
    // now figure out which third of the spectrum we're in:
    if( t192 & 0x80) {
        // we're in the hottest third
        heatcolor.r = 255; // full red
        heatcolor.g = 255; // full green
        heatcolor.b = heatramp; // ramp up blue

    } else if( t192 & 0x40 ) {
        // we're in the middle third
        heatcolor.r = 255; // full red
        heatcolor.g = heatramp; // ramp up green
        heatcolor.b = 0; // no blue

    } else {
        // we're in the coolest third
        heatcolor.r = heatramp; // ramp up red
        heatcolor.g = 0; // no green
        heatcolor.b = 0; // no blue
    }

    return heatcolor;
}

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


class Fire : public Program
{
public:
    Fire()
        : Program(10)
    {
        heat = new uint8_t[NUM_LEDS];
    }

    virtual ~Fire()
    {
        delete[] heat;
    }

    virtual bool run()
    {
        if (limiter.skip()) return false;

        // Step 1.  Cool down every cell a little
        for (int i = 0; i < NUM_LEDS; i++)
            heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS_PER_STRAND) + 2));
  
        // Step 2.  Heat from each cell drifts 'up' and diffuses a little
        for (int s = 0; s < NUM_OF_STRANDS; ++s)
        {
            const int offset = s * NUM_LEDS_PER_STRAND;
            for (int k = NUM_LEDS_PER_STRAND - 1; k >= 2; k--)
                heat[offset + k] = (heat[offset + k - 1] + heat[offset + k - 2] + heat[offset + k - 2]) / 3;
        }
        // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
        for (int s = 0; s < NUM_OF_STRANDS; ++s)
        {
            const int offset = s * NUM_LEDS_PER_STRAND;
            if (random8() < SPARKING)
            {
                int y = random8(7);
                heat[offset + y] = qadd8(heat[offset + y], random8(160, 255));
            }
        }

        // Step 4.  Map from heat cells to LED colors
        for (int j = 0; j < NUM_LEDS; j++)
            set_pixel(j, HeatColor(heat[j]));

        return true;
    }

private:
    // Array of temperature readings at each simulation cell
    uint8_t* heat;
};

REGISTER_PROGRAM(Fire);
