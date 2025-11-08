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
    
    Vec3 position;
    Vec3 rotation; 
    Vec3 scale;
    
    int color_mode;  // 0=gradient, 1=checker, 2=pure
    unsigned char solid_r, solid_g, solid_b;
    
    void GenerateGradientColors(std::vector<Triangle>& triangles);
    void GenerateCheckeredColors(std::vector<Triangle>& triangles);
    void GenerateSolidColors(std::vector<Triangle>& triangles);

  public:
    MeshCube();
    
    Matrix GetModelMatrix() const;
    std::vector<Triangle> GetTriangles();
    
    void SetPosition(const Vec3& pos);
    void SetRotation(const Vec3& rot);
    void SetScale(const Vec3& scale);
    
    void SetColorMode(int mode);
    void SetSolidColor(unsigned char r, unsigned char g, unsigned char b);
};