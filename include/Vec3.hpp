#pragma once

// three dimension vector struture
class Vec3 {
  public:
    float x, y, z;

  public:
    Vec3(float x = 0, float y = 0, float z = 0);

    // basic operation
    Vec3 operator+(const Vec3 &rhs) const noexcept;
    Vec3 operator-(const Vec3 &rhs) const noexcept;
    Vec3 operator*(float m) const noexcept;
    Vec3 operator/(float m) const noexcept;
    Vec3 operator-() const noexcept;

    // dot operation
    float dot(const Vec3 &rhs) const noexcept;

    // cross operation
    Vec3 cross(const Vec3 &rhs) const noexcept;

    // normalize
    Vec3 normalize() const noexcept;
    
    Vec3 toVec3() const noexcept;
    Vec3 toVec3Homogeneous() const;
};
