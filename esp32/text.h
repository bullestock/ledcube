#pragma once

#include <Arduino.h>

#include "chasecolours.hpp"

class Text
{
public:
    Text(const char* t)
        : text(t)
    {
    }

    // Show next letter of text string
    void show();

private:
    ChaseColours cc;
    String text;
    int idx = 0;
    int row = 0;
};
