#pragma once

#include "Camera.hpp"
#include "Matrix.hpp"
#include "MeshCube.hpp"
#include "PPM.hpp"
#include <vector>

class Render {
  public:
    Render(int width, int height);
    ~Render();

    void RenderMesh(MeshCube &mesh, Camera &camera, PPM &ppm);

    void ClearDepthBuffer();

    void SetViewport(int width, int height);

  private:
    Vec4 TransformVertex(const Vec4 &vertex, const Matrix &mvp);

    std::vector<Triangle> ClipTriangle(const Triangle &tri);

    Vec4 PerspectiveDivide(const Vec4 &vertex);

    Vec4 ViewportTransform(const Vec4 &vertex);

    void RasterizeWithDepthTest(const Triangle &tri, PPM &ppm);

    bool DepthTest(int x, int y, float depth);

    void WriteDepth(int x, int y, float depth);

    bool IsInsidePlane(const Vec4 &vertex, int plane);
    Vec4 IntersectPlane(const Vec4 &v1, const Vec4 &v2, int plane);
    std::vector<Triangle> ClipAgainstPlane(const std::vector<Triangle> &triangles, int plane);

  private:
    int m_width;
    int m_height;
    std::vector<float> m_depth_buffer;
};