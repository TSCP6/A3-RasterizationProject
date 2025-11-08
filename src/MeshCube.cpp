#include "MeshCube.hpp"
#include <cmath>

MeshCube::MeshCube()
    : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1), color_mode(0), solid_r(255), solid_g(255), solid_b(255) {
    vertices.clear();
    vertices.push_back({-1.0f, -1.0f, -1.0f}); // 0
    vertices.push_back({1.0f, -1.0f, -1.0f});  // 1
    vertices.push_back({1.0f, 1.0f, -1.0f});   // 2
    vertices.push_back({-1.0f, 1.0f, -1.0f});  // 3
    vertices.push_back({-1.0f, -1.0f, 1.0f});  // 4
    vertices.push_back({1.0f, -1.0f, 1.0f});   // 5
    vertices.push_back({1.0f, 1.0f, 1.0f});    // 6
    vertices.push_back({-1.0f, 1.0f, 1.0f});   // 7

    tri_index = {
        0, 2, 1,  0, 3, 2,
        4, 5, 6,  4, 6, 7,
        0, 7, 3,  0, 4, 7,
        1, 6, 5,  1, 2, 6,
        0, 5, 4,  0, 1, 5,
        3, 6, 2,  3, 7, 6
    };
}

void MeshCube::SetPosition(const Vec3 &pos) {
    position = pos;
}

void MeshCube::SetRotation(const Vec3 &rot) {
    rotation = rot;
}

void MeshCube::SetScale(const Vec3 &sc) {
    scale = sc;
}

void MeshCube::SetColorMode(int mode) {
    color_mode = mode;
}

void MeshCube::SetSolidColor(unsigned char r, unsigned char g, unsigned char b) {
    solid_r = r;
    solid_g = g;
    solid_b = b;
}

// returns a matrix to transform model position to world position
Matrix MeshCube::GetModelMatrix() const {
    // zoom the matrix
    Matrix m_scale = Matrix::scale(scale.x, scale.y, scale.z);

    // rotation matrix
    float rx = rotation.x * 3.14159265f / 180.0f;
    float ry = rotation.y * 3.14159265f / 180.0f;
    float rz = rotation.z * 3.14159265f / 180.0f;

    Matrix rotX = Matrix::rotationX(rx);
    Matrix rotY = Matrix::rotationY(ry);
    Matrix rotZ = Matrix::rotationZ(rz);

    // translation
    Matrix translation = Matrix::translation(position.x, position.y, position.z);

    // Model = Translation × RotationZ × RotationY × RotationX × Scale
    return translation * rotZ * rotY * rotX * m_scale;
}

std::vector<Triangle> MeshCube::GetTriangles() {
    std::vector<Triangle> triangles;

    for (size_t i = 0; i < tri_index.size(); i += 3) {
        Triangle tri;

        const Vertex &v0 = vertices[tri_index[i]];
        const Vertex &v1 = vertices[tri_index[i + 1]];
        const Vertex &v2 = vertices[tri_index[i + 2]];

        tri.v_3[0] = Vec3(v0.x, v0.y, v0.z);
        tri.v_3[1] = Vec3(v1.x, v1.y, v1.z);
        tri.v_3[2] = Vec3(v2.x, v2.y, v2.z);

        tri.r = 255;
        tri.g = 255;
        tri.b = 255;

        triangles.push_back(tri);
    }

    switch (color_mode) {
    case 0:
        GenerateGradientColors(triangles);
        break;
    case 1:
        GenerateCheckeredColors(triangles);
        break;
    }

    return triangles;
}

void MeshCube::GenerateGradientColors(std::vector<Triangle> &triangles) {
    auto map01 = [](float v) -> unsigned char {
        float c = (v + 1.0f) * 0.5f; // [-1,1] -> [0,1]
        c = std::max(0.0f, std::min(1.0f, c));
        return static_cast<unsigned char>(c * 255.0f);
    };

    for (auto &tri : triangles) {
        for (int i = 0; i < 3; ++i) {
            Vec3 &v = tri.v_3[i];
            tri.vr[i] = map01(v.x); // R <- x
            tri.vg[i] = map01(v.y); // G <- y
            tri.vb[i] = map01(v.z); // B <- z
        }

        Vec3 center = (tri.v_3[0] + tri.v_3[1] + tri.v_3[2]) / 3.0f;
        tri.r = map01(center.x);
        tri.g = map01(center.y);
        tri.b = map01(center.z);
    }
}

void MeshCube::GenerateCheckeredColors(std::vector<Triangle> &triangles) {
    unsigned char colors[6][3] = {
        {100, 150, 255}, {150, 255, 150}, {255, 150, 150},
        {255, 255, 150}, {150, 150, 255}, {255, 200, 150}
    };

    for (size_t i = 0; i < triangles.size(); i++) {
        int face = i / 2;
        triangles[i].r = colors[face][0];
        triangles[i].g = colors[face][1];
        triangles[i].b = colors[face][2];

        for (int v = 0; v < 3; ++v) {
            triangles[i].vr[v] = colors[face][0];
            triangles[i].vg[v] = colors[face][1];
            triangles[i].vb[v] = colors[face][2];
        }
    }
}