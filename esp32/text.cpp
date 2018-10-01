#include <Arduino.h>
#include "program.hpp"
#include "chargen.h"
#include "display.hpp"
#include "text.h"
#include "random8.h"

void Text::show()
{
    if (row >= NUM_LEDS_PER_ROW)
    {
        clear_all();
        row = 0;
        const int skip = random8(1, 3);
        for (int i = 0; i < skip; ++i)
            cc.next();
        if (++idx >= text.length())
            idx = 0;
    }
    CharacterGenerator cg;
    cg.draw(AXIS_Y, row, text[idx], cc.get());
    ++row;
}
