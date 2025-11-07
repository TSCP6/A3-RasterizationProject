#pragma once
#include "Vec2.hpp"

// triangle structure
struct Triangle {
  public:
    Vec2 v[3];             // three vertex
    unsigned char r, g, b; // three colors

  public:
    Triangle(Vec2 v0, Vec2 v1, Vec2 v2, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255);
};