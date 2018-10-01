#pragma once

#include "display.hpp"

// A rectangular object that can be moved in XYZ.
class Block
{
public:
    Block(pixelColor_t colour,
          int cx, int cy, int cz,
          int x_size, int y_size, int z_size);

    void show();
    
    // Return true if the move is possible.
    bool move(Direction dir, int amount);

    void set_colour(pixelColor_t colour);
    
private:
    void compute_extent(const Position& p,
                        Position& p1,
                        Position& p2) const;

    void compute_extent(Position& p1,
                        Position& p2) const;

    void show(pixelColor_t c);
    
    pixelColor_t col;
    Position pos;
    Position size; // ah well
};
