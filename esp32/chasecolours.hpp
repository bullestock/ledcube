#pragma once

#include "display.hpp"

class ChaseColours
{
public:
    static pixelColor_t get()
    {
        return chase_colours[idx];
    }

    static void next();

private:
    static int idx;
    static const pixelColor_t chase_colours[];
};
