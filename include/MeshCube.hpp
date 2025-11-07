#pragma once
#include "Matrix.hpp"
#include "Triangle.hpp"
#include "Vec3.hpp"
#include <vector>

struct Vertex{
    float x, y, z;
};

class MeshCube {
  private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> tri_index;
    
    Vec3 m_position;
    Vec3 m_rotation;  // 欧拉角（度）
    Vec3 m_scale;
    
    int m_color_mode;  // 0=渐变, 1=棋盘格, 2=纯色
    unsigned char m_solid_r, m_solid_g, m_solid_b;
    
    void GenerateGradientColors(std::vector<Triangle>& triangles);
    void GenerateCheckeredColors(std::vector<Triangle>& triangles);
    void GenerateSolidColors(std::vector<Triangle>& triangles);

  public:
    MeshCube();
    
    void createCube();
    Matrix GetModelMatrix() const;
    std::vector<Triangle> GetTriangles();
    
    // 变换设置
    void SetPosition(const Vec3& pos);
    void SetRotation(const Vec3& rot);
    void SetScale(const Vec3& scale);
    
    // 颜色模式
    void SetColorMode(int mode);
    void SetSolidColor(unsigned char r, unsigned char g, unsigned char b);
};