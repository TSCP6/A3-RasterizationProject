#include "MeshCube.hpp"
#include <cmath>

MeshCube::MeshCube() 
    : m_position(0, 0, 0),
      m_rotation(0, 0, 0),
      m_scale(1, 1, 1),
      m_color_mode(0),
      m_solid_r(255), m_solid_g(255), m_solid_b(255) {
    createCube();
}

void MeshCube::createCube() {
    vertices.clear();
    tri_index.clear();

    vertices = {
        {-0.5f, -0.5f,  0.5f},  // 0: 左下前
        { 0.5f, -0.5f,  0.5f},  // 1: 右下前
        { 0.5f,  0.5f,  0.5f},  // 2: 右上前
        {-0.5f,  0.5f,  0.5f},  // 3: 左上前
        
        {-0.5f, -0.5f, -0.5f},  // 4: 左下后
        { 0.5f, -0.5f, -0.5f},  // 5: 右下后
        { 0.5f,  0.5f, -0.5f},  // 6: 右上后
        {-0.5f,  0.5f, -0.5f}   // 7: 左上后
    };

    //use every 3 vertices to save a triangle
    tri_index = {
        // 前面 (+Z)
        0, 1, 2,    0, 2, 3,
        
        // 后面 (-Z)
        5, 4, 7,    5, 7, 6,
        
        // 右面 (+X)
        1, 5, 6,    1, 6, 2,
        
        // 左面 (-X)
        4, 0, 3,    4, 3, 7,
        
        // 上面 (+Y)
        3, 2, 6,    3, 6, 7,
        
        // 下面 (-Y)
        4, 5, 1,    4, 1, 0
    };
}

void MeshCube::SetPosition(const Vec3& pos) {
    m_position = pos;
}

void MeshCube::SetRotation(const Vec3& rot) {
    m_rotation = rot;
}

void MeshCube::SetScale(const Vec3& scale) {
    m_scale = scale;
}

void MeshCube::SetColorMode(int mode) {
    m_color_mode = mode;
}

void MeshCube::SetSolidColor(unsigned char r, unsigned char g, unsigned char b) {
    m_solid_r = r;
    m_solid_g = g;
    m_solid_b = b;
}

Matrix MeshCube::GetModelMatrix() const {
    // 缩放矩阵
    Matrix scale = Matrix::scale(m_scale.x, m_scale.y, m_scale.z);
    
    // 旋转矩阵（角度转弧度）
    float rx = m_rotation.x * 3.14159265f / 180.0f;
    float ry = m_rotation.y * 3.14159265f / 180.0f;
    float rz = m_rotation.z * 3.14159265f / 180.0f;
    
    Matrix rotX = Matrix::rotationX(rx);
    Matrix rotY = Matrix::rotationY(ry);
    Matrix rotZ = Matrix::rotationZ(rz);
    
    // 平移矩阵
    Matrix translation = Matrix::translation(m_position.x, m_position.y, m_position.z);
    
    // Model = Translation × RotationZ × RotationY × RotationX × Scale
    return translation * rotZ * rotY * rotX * scale;
}

std::vector<Triangle> MeshCube::GetTriangles() {
    std::vector<Triangle> triangles;
    
    // 将顶点和索引转换为三角形
    for (size_t i = 0; i < tri_index.size(); i += 3) {
        Triangle tri;
        
        // 设置3D顶点
        const Vertex& v0 = vertices[tri_index[i]];
        const Vertex& v1 = vertices[tri_index[i + 1]];
        const Vertex& v2 = vertices[tri_index[i + 2]];
        
        tri.v_3[0] = Vec3(v0.x, v0.y, v0.z);
        tri.v_3[1] = Vec3(v1.x, v1.y, v1.z);
        tri.v_3[2] = Vec3(v2.x, v2.y, v2.z);
        
        // 默认颜色（将被颜色模式覆盖）
        tri.r = 255;
        tri.g = 255;
        tri.b = 255;
        
        triangles.push_back(tri);
    }
    
    // 根据颜色模式设置颜色
    switch(m_color_mode) {
        case 0: GenerateGradientColors(triangles); break;
        case 1: GenerateCheckeredColors(triangles); break;
        case 2: GenerateSolidColors(triangles); break;
    }
    
    return triangles;
}

void MeshCube::GenerateGradientColors(std::vector<Triangle>& triangles) {
    for (auto& tri : triangles) {
        // 计算三角形中心
        Vec3 center = (tri.v_3[0] + tri.v_3[1] + tri.v_3[2]) / 3.0f;
        
        // 映射到颜色（范围[-0.5, 0.5] → [0, 255]）
        tri.r = static_cast<unsigned char>((center.x + 0.5f) * 255);
        tri.g = static_cast<unsigned char>((center.y + 0.5f) * 255);
        tri.b = static_cast<unsigned char>((center.z + 0.5f) * 255);
    }
}

void MeshCube::GenerateCheckeredColors(std::vector<Triangle>& triangles) {
    // 6个面的颜色
    unsigned char colors[6][3] = {
        {100, 150, 255},  // 前面：蓝
        {150, 255, 150},  // 后面：绿
        {255, 150, 150},  // 右面：红
        {255, 255, 150},  // 左面：黄
        {150, 150, 255},  // 上面：紫
        {255, 200, 150}   // 下面：橙
    };
    
    for (size_t i = 0; i < triangles.size(); i++) {
        int face = i / 2;  // 每个面有2个三角形
        triangles[i].r = colors[face][0];
        triangles[i].g = colors[face][1];
        triangles[i].b = colors[face][2];
    }
}

void MeshCube::GenerateSolidColors(std::vector<Triangle>& triangles) {
    for (auto& tri : triangles) {
        tri.r = m_solid_r;
        tri.g = m_solid_g;
        tri.b = m_solid_b;
    }
}