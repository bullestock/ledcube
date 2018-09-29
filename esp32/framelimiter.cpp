/*
 * Neomatrix - Copyright (C) 2017 Lars Christensen
 * MIT Licensed
 *
 * Frame limiter
 */

#include <Arduino.h>
#include "framelimiter.hpp"

FrameLimiter::FrameLimiter(float fps)
{
    last_frame = millis();
    setFps(fps);
}

void FrameLimiter::setFps(float f)
{
    fps = f;
    ms_per_frame = 1000 / fps;
}

bool FrameLimiter::skip()
{
  const auto now = millis();
  if (now - last_frame < ms_per_frame)
      return true;
  
  last_frame = now;

  return false;
}

