#include <Arduino.h>
#include "program.hpp"
#include "chargen.h"
#include "display.hpp"
#include "text.h"

void Text::show()
{
    if (row >= NUM_LEDS_PER_ROW)
    {
        clear_all();
        row = 0;
        cc.next();
        if (++idx >= text.length())
            idx = 0;
    }
    CharacterGenerator cg;
    cg.draw(AXIS_Y, row, text[idx], cc.get());
    ++row;
}
