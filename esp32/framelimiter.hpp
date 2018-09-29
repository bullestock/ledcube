/*
 * Neomatrix - Copyright (C) 2017 Lars Christensen
 * MIT Licensed
 *
 * Frame limiter
 */

#pragma once

#include <stdint.h>

class FrameLimiter
{
public:
    FrameLimiter(float fps);
    bool skip();

    void setFps(float fps);
    
private:
    float fps = 1;
    unsigned long ms_per_frame = 0;
    unsigned long last_frame = 0;
};

