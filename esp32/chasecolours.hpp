#pragma once

#include "display.hpp"

class ChaseColours
{
public:
    pixelColor_t get()
    {
        return chase_colours[idx];
    }

    void next();

private:
    int idx = 0;
    static const pixelColor_t chase_colours[];
};
