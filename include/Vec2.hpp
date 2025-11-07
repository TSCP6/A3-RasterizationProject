#pragma once

// two dimension vector structure
class Vec2 {
  public:
    float x, y;

  public:
    Vec2(float x = 0, float y = 0);

    // basic operation
    Vec2 operator+(const Vec2 &rhs) const noexcept;
    Vec2 operator-(const Vec2 &rhs) const noexcept;
    Vec2 operator*(float m) const noexcept;
    Vec2 operator-() const noexcept;

    // dot operation
    float dot(const Vec2 &rhs) const noexcept;

    // normalize
    Vec2 normalize() const noexcept;
};