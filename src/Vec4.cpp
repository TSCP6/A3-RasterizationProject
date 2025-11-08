#include "Vec4.hpp"
#include "Vec3.hpp"
#include "cmath"

Vec4::Vec4() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {
}

Vec4::Vec4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {
}

// Construct from Vec3 (for positions, w=1.0)
Vec4::Vec4(const Vec3 &v, float w) noexcept : x(v.x), y(v.y), z(v.z), w(w) {
}

// Copy constructor
Vec4::Vec4(const Vec4 &other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {
}

// Assignment operator
Vec4 &Vec4::operator=(const Vec4 &other) noexcept {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
    return *this;
}

Vec4 Vec4::operator+(const Vec4 &other) const noexcept {
    return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vec4 Vec4::operator-(const Vec4 &other) const noexcept {
    return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vec4 Vec4::operator*(float scalar) const noexcept {
    return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vec4 Vec4::operator-() const noexcept {
    return Vec4(-x, -y, -z, -w);
}

float Vec4::dot(const Vec4 &other) const noexcept {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}
Vec4 Vec4::normalize() const {
    float len = std::sqrt(x * x + y * y + z * z + w * w);
    return Vec4(x / len, y / len, z / len, w / len);
}

Vec4 Vec4::perspectiveDivide() const {
    if (std::abs(w) < 1e-6f) { // avoid divide 0
        return Vec4(x, y, z, w);
    }
    float invW = 1.0f / w;
    return Vec4(x * invW, y * invW, z * invW, 1.0f);
}