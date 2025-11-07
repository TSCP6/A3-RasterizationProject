#include "Vec2.hpp"

#include <math.h>

Vec2::Vec2(float x, float y) : x(x), y(y) {
}

Vec2 Vec2::operator+(const Vec2 &rhs) const noexcept {
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const noexcept {
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(float m) const noexcept {
    return Vec2(x * m, y * m);
}

Vec2 Vec2::operator-() const noexcept {
    return Vec2(-x, -y);
}

float Vec2::dot(const Vec2 &rhs) const noexcept {
    return x * rhs.x + y * rhs.y;
}

Vec2 Vec2::normalize() const noexcept {
    float magnitude = sqrt(x * x + y * y);
    return Vec2(x / magnitude, y / magnitude);
}