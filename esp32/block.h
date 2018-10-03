#pragma once

#include "display.hpp"

// A rectangular object that can be moved in XYZ.
class Block
{
public:
    Block(pixelColor_t colour = pixelFromHex(White),
          int cx = 0, int cy = 0, int cz = 0,
          int x_size = 0, int y_size = 0, int z_size = 0);

    void show();
    
    // Return true if the move is possible.
    bool move(Direction dir, int amount);

    void set_colour(pixelColor_t colour);

    Position position() const;

    void set_position(const Position&);

    void set_size(const Position&);

    bool contains(const Position&) const;

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
