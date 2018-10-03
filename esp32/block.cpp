#include <Arduino.h>
#include "block.h"

Block::Block(pixelColor_t colour,
             int cx, int cy, int cz,
             int x_size, int y_size, int z_size)
    : col(colour),
      pos(cx, cy, cz),
      size(x_size, y_size, z_size)
{
}

void Block::show()
{
    show(col);
}

void Block::show(pixelColor_t c)
{
    Position p1, p2;
    compute_extent(p1, p2);
    if (!is_inside(p1) || !is_inside(p2))
    {
        Serial.print("Block size ");
        Serial.print(size.x);
        Serial.print(", ");
        Serial.print(size.y);
        Serial.print(", ");
        Serial.print(size.z);
        Serial.print(" at ");
        Serial.print(pos.x);
        Serial.print(", ");
        Serial.print(pos.y);
        Serial.print(", ");
        Serial.print(pos.z);
        Serial.print(": show: p1 ");
        Serial.print(p1.x);
        Serial.print(", ");
        Serial.print(p1.y);
        Serial.print(", ");
        Serial.print(p1.z);
        Serial.print(": show: p2 ");
        Serial.print(p2.x);
        Serial.print(", ");
        Serial.print(p2.y);
        Serial.print(", ");
        Serial.println(p2.z);
        internal_error();
    }
    for (int x = p1.x; x <= p2.x; ++x)
        for (int y = p1.y; y <= p2.y; ++y)
            for (int z = p1.z; z <= p2.z; ++z)
                set_pixel(x, y, z, c);
}

bool Block::move(Direction dir, int amount)
{
    auto new_pos = translate(pos, dir, amount);
    Position p1, p2;
    compute_extent(new_pos, p1, p2);
    if (!is_inside(p1) || !is_inside(p2))
        return false;
    show(pixelFromRGB(0, 0, 0));
    pos = new_pos;
    show();
    return true;
}

void Block::set_colour(pixelColor_t colour)
{
    col = colour;
    show();
}

Position Block::position() const
{
    return pos;
}

void Block::set_position(const Position& p)
{
    pos = p;
}

void Block::set_size(const Position& p)
{
    size = p;
}

bool Block::contains(const Position& p) const
{
    Position p1, p2;
    compute_extent(p1, p2);
    return (p1.x <= p.x) && (p.x <= p2.x) &&
       (p1.y <= p.y) && (p.y <= p2.y) &&
       (p1.z <= p.z) && (p.z <= p2.z);
}

void Block::compute_extent(const Position& p,
                           Position& p1,
                           Position& p2) const
{
    p1.x = p.x - size.x/2;
    p1.y = p.y - size.y/2;
    p1.z = p.z - size.z/2;
    p2.x = p.x + size.x/2 - (size.x % 2 ? 0 : 1);
    p2.y = p.y + size.y/2 - (size.y % 2 ? 0 : 1);
    p2.z = p.z + size.z/2 - (size.z % 2 ? 0 : 1);
}

void Block::compute_extent(Position& p1,
                           Position& p2) const
{
    compute_extent(pos, p1, p2);
}
