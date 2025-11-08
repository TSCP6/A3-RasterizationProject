#include "Render.hpp"

#include <algorithm>
#include <limits>

Render::Render(int width, int height) : width(width), height(height) {
    depth_buffer.resize(width * height);
    ClearDepthBuffer();
}

Render::~Render() {
}

void Render::ClearDepthBuffer() {
    std::fill(depth_buffer.begin(), depth_buffer.end(), std::numeric_limits<float>::infinity());
}

void Render::SetViewport(int width, int height) {
    width = width;
    height = height;
    depth_buffer.resize(width * height);
    ClearDepthBuffer();
}

void Render::RenderMesh(MeshCube &mesh, Camera &camera, PPM &ppm) {
    // use mvp get the clip space position
    Matrix model = mesh.GetModelMatrix();
    Matrix view = camera.getViewMatrix();
    Matrix projection = camera.getProjectionMatrix();
    Matrix mvp = projection * view * model;

    // get mesh triangles
    std::vector<Triangle> triangles = mesh.GetTriangles();

    // traverse all triangles
    for (auto &tri : triangles) {
        Triangle transformed_tri = tri;
        for (int i = 0; i < 3; i++) {
            // transform vec3 into vec4 with w=1.0
            Vec4 vertex = Vec4(tri.v_3[i], 1.0);

            // mvp transformation
            Vec4 clip_space = mvp * vertex;

            if (std::abs(clip_space.w) > 1e-6f) {
                float inv_w = 1.0f / clip_space.w;

                transformed_tri.v_3[i].x = clip_space.x * inv_w;
                transformed_tri.v_3[i].y = clip_space.y * inv_w;
                transformed_tri.v_3[i].z = clip_space.z * inv_w;

                transformed_tri.w_clip[i] = inv_w;
                transformed_tri.depth_over_w[i] = clip_space.z * inv_w;

                // map the depth into [0,1]
                transformed_tri.depth[i] = (clip_space.z / clip_space.w + 1.0f) * 0.5f;
            } else {
                transformed_tri.depth[i] = std::numeric_limits<float>::infinity();
                transformed_tri.w_clip[i] = 0.0f;
                transformed_tri.depth_over_w[i] = 0.0f;
            }
        }

        // clip check
        bool all_behind = true;
        bool all_outside_x = true;
        bool all_outside_y = true;

        for (int i = 0; i < 3; i++) {
            float x = transformed_tri.v_3[i].x;
            float y = transformed_tri.v_3[i].y;
            float z = transformed_tri.depth[i];

            if (z >= 0.0f && z <= 1.0f)
                all_behind = false;
            if (x >= -1.0f && x <= 1.0f)
                all_outside_x = false;
            if (y >= -1.0f && y <= 1.0f)
                all_outside_y = false;
        }

        // skip the triangles fully behind the screen
        if (all_behind || all_outside_x || all_outside_y) {
            continue;
        }

        // NDC is [-1,1]×[-1,1]，map to [0,1]×[0,1]
        for (int i = 0; i < 3; i++) {
            float screen_x = (transformed_tri.v_3[i].x + 1.0f) * 0.5f;
            float screen_y = (1.0f - transformed_tri.v_3[i].y) * 0.5f; // 翻转Y轴
            transformed_tri.v_2[i] = Vec2(screen_x, screen_y);
        }

        // 9.rasterization
        ppm.RasterizeTriangleWithDepth(transformed_tri);
    }
}