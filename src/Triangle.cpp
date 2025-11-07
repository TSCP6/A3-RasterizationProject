#include "Triangle.hpp"

Triangle::Triangle() : r(255), g(255), b(255) {
    for (int i = 0; i < 3; i++) {
        v_2[i] = Vec2(0, 0);
        v_3[i] = Vec3(0, 0, 0);
        depth[i] = 0.0f;
    }
}

Triangle::Triangle(Vec2 v0, Vec2 v1, Vec2 v2, unsigned char r, unsigned char g, unsigned char b)
    : r(r), g(g), b(b) {
    v_2[0] = v0;
    v_2[1] = v1;
    v_2[2] = v2;
    for (int i = 0; i < 3; i++) {
        depth[i] = 0.0f;
    }
}