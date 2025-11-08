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
    int width;
    int height;
    std::vector<float> depth_buffer;
};