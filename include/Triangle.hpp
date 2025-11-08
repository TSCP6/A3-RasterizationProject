#pragma once
#include "Vec2.hpp"
#include "Vec3.hpp"

// triangle structure
struct Triangle {
  public:
    Vec2 v_2[3]; // three vertex
    Vec3 v_3[3];
    float depth[3]; // vertex depth

    float w_clip[3];       // 1/w from clip space
    float depth_over_w[3]; // z/w from clip space

    unsigned char r, g, b; // three colors
    unsigned char vr[3], vg[3], vb[3];

  public:
    Triangle();
    Triangle(Vec2 v0, Vec2 v1, Vec2 v2, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255);
};