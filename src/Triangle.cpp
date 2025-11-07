#include "Triangle.hpp"

Triangle::Triangle(Vec2 v0, Vec2 v1, Vec2 v2, unsigned char r, unsigned char g, unsigned char b)
    : r(r), g(g), b(b) {
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
}