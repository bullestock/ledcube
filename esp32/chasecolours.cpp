#include "chasecolours.hpp"

int ChaseColours::idx = 0;
const pixelColor_t ChaseColours::chase_colours[] = {
    pixelFromHex(Yellow), pixelFromHex(Green), pixelFromHex(HotPink), pixelFromHex(Blue), pixelFromHex(Red), pixelFromHex(White)
};

void ChaseColours::next()
{
    ++idx;
    if (idx >= sizeof(chase_colours)/sizeof(pixelColor_t))
        idx = 0;
}
