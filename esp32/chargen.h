#pragma once

#include "display.hpp"

class CharacterGenerator
{
public:
    CharacterGenerator();

    void draw(Axis axis, int row, char symbol, pixelColor_t colour);
};
