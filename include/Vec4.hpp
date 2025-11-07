#pragma once

#include "Vec3.hpp"

// four dimension vector structure
class Vec4 {
  public:
    float x, y, z;
    // w=1.0 means can be changed when translating, like vertex
    // w=0.0 means can't change in translation, like normal vector;
    // in projection, w=-z means the depth of vertex
    float w;

  public:
    Vec4() noexcept;
    Vec4(float x, float y, float z, float w = 1.0f) noexcept;
    Vec4(const Vec3 &v, float w = 1.0f) noexcept;
    Vec4(const Vec4 &other) noexcept;

    Vec4 &operator=(const Vec4 &other) noexcept;

    // Array access
    float &operator[](int index) noexcept;
    const float &operator[](int index) const noexcept;

    Vec4 operator+(const Vec4 &other) const noexcept;
    Vec4 operator-(const Vec4 &other) const noexcept;
    Vec4 operator*(float scalar) const noexcept;
    Vec4 operator-() const noexcept;

    // Dot operation
    float dot(const Vec4 &other) const noexcept;

    Vec4 normalize() const;

    Vec4 perspectiveDivide() const;
    Vec3 toNDC() const;

    // Comparison
    bool operator==(const Vec4 &other) const noexcept;
    bool operator!=(const Vec4 &other) const noexcept;

    static Vec4 zero() noexcept;
    static Vec4 one() noexcept;
    static float dot(const Vec4 &a, const Vec4 &b) noexcept;
};