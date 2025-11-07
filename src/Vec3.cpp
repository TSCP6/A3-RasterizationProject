#include "Vec3.hpp"

#include <cmath>

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {
}

Vec3 Vec3::operator+(const Vec3 &rhs) const noexcept {
    return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vec3 Vec3::operator-(const Vec3 &rhs) const noexcept {
    return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vec3 Vec3::operator*(float m) const noexcept {
    return Vec3(x * m, y * m, z * m);
}

Vec3 Vec3::operator/(float m) const noexcept {
    return Vec3(x / m, y / m, z / m);
}

Vec3 Vec3::operator-() const noexcept {
    return Vec3(-x, -y, -z);
}

float Vec3::dot(const Vec3 &rhs) const noexcept {
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vec3 Vec3::cross(const Vec3 &rhs) const noexcept {
    float cross_x = y * rhs.z - rhs.y * z;
    float cross_y = rhs.x * z - x * rhs.z;
    float cross_z = x * rhs.y - rhs.x * y;
    return Vec3(cross_x, cross_y, cross_z); 
}

Vec3 Vec3::normalize() const noexcept {
    float magnitude = sqrt(x * x + y * y + z * z);
    return Vec3(x / magnitude, y / magnitude, z / magnitude);
}