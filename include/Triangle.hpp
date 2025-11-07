#pragma once
#include "Vec2.hpp"
#include "Vec3.hpp"

// triangle structure
struct Triangle {
  public:
    Vec2 v_2[3];             // three vertex
    Vec3 v_3[3];
    float depth[3];        // vertex depth
    unsigned char r, g, b; // three colors

  public:
    Triangle();  // 添加默认构造函数
    Triangle(Vec2 v0, Vec2 v1, Vec2 v2, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255);
};