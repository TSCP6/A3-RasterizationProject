#include "PPM.hpp"

#include <algorithm>
#include <fstream>

PPM::PPM(int w, int h, int ss) : width(w), height(h), ss_factor(ss) {
    // resize function's second parameter fills the empty array members
    data.resize(w * h * 3, 255);
    hr_buffer.resize((w * ss) * (h * ss) * 3, 255);
    depth_buffer.resize(w * h, std::numeric_limits<float>::infinity());
    hr_depth_buffer.resize((w * ss) * (h * ss), std::numeric_limits<float>::infinity());
}

void PPM::BeginFrame() {
    std::fill(data.begin(), data.end(), static_cast<unsigned char>(255));
    std::fill(hr_buffer.begin(), hr_buffer.end(), static_cast<unsigned char>(255));
    ClearDepthBuffer();
}

// assign all pixels' depth as infinity
void PPM::ClearDepthBuffer() {
    std::fill(depth_buffer.begin(), depth_buffer.end(), std::numeric_limits<float>::infinity());
    std::fill(hr_depth_buffer.begin(), hr_depth_buffer.end(), std::numeric_limits<float>::infinity());
}

void PPM::ResolveSSAA() {
    if (ss_factor <= 1)
        return;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int normal_index = (width * y + x) * 3;
            for (int k = 0; k < 3; k++) {
                int hr_sum = 0;
                for (int i = 0; i < ss_factor; i++)
                    for (int j = 0; j < ss_factor; j++) {
                        int sx = x * ss_factor + j;
                        int sy = y * ss_factor + i;
                        int hr_index = (width * ss_factor * sy + sx) * 3 + k;
                        hr_sum += static_cast<int>(hr_buffer[hr_index]);
                    }
                unsigned char average = static_cast<unsigned char>(hr_sum / (ss_factor * ss_factor));
                data[normal_index + k] = average;
            }
        }
    }
}

bool PPM::SetPixelWithDepth(int x, int y, float depth, unsigned char r, unsigned char g, unsigned char b) {
    if (ss_factor > 1) {
        if (x >= 0 && x < width * ss_factor && y >= 0 && y < height * ss_factor) {
            int index = ss_factor * width * y + x;
            if (depth < hr_depth_buffer[index]) {
                hr_depth_buffer[index] = depth;
                hr_buffer[index * 3 + 0] = r;
                hr_buffer[index * 3 + 1] = g;
                hr_buffer[index * 3 + 2] = b;
                return true;
            }
        }
    } else {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int index = width * y + x;
            if (depth < depth_buffer[index]) {
                depth_buffer[index] = depth;
                data[index * 3 + 0] = r;
                data[index * 3 + 1] = g;
                data[index * 3 + 2] = b;
                return true;
            }
        }
    }
    return false;
}

void PPM::SetPixelHighRes(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x >= 0 && x < width * ss_factor && y >= 0 && y < height * ss_factor) {
        int index = (ss_factor * width * y + x) * 3;
        hr_buffer[index + 0] = r;
        hr_buffer[index + 1] = g;
        hr_buffer[index + 2] = b;
    }
}

// fill the (x, y) pixel with color (r, g, b)
void PPM::SetPixelLowRes(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        int index = (width * y + x) * 3;
        data[index + 0] = r;
        data[index + 1] = g;
        data[index + 2] = b;
    }
}

void PPM::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (ss_factor > 1) {
        SetPixelHighRes(x, y, r, g, b);
    } else {
        SetPixelLowRes(x, y, r, g, b);
    }
}

// transform the origin position to screen position
// the origin position is [0, 1]×[0, 1]
Vec2 PPM::WorldToScreen(const Vec2 &world) {
    float screenX = world.x * width;
    float screenY = world.y * height;
    return Vec2(screenX, screenY);
}

// the cross mutiplication of vec2 v2v0 v1v0, and judge which side v2 is of v0v1
float PPM::EdgeFunction(const Vec2 &v0, const Vec2 &v1, const Vec2 &v2) {
    return (v2.x - v0.x) * (v1.y - v0.y) - (v2.y - v0.y) * (v1.x - v0.x);
}

void PPM::RasterizeTriangleWithDepth(const Triangle &tri) {
    // turn to screen position 
    Vec2 v0 = WorldToScreen(tri.v_2[0]);
    Vec2 v1 = WorldToScreen(tri.v_2[1]);
    Vec2 v2 = WorldToScreen(tri.v_2[2]);

    if (ss_factor > 1) {
        v0.x *= ss_factor;
        v0.y *= ss_factor;
        v1.x *= ss_factor;
        v1.y *= ss_factor;
        v2.x *= ss_factor;
        v2.y *= ss_factor;
    }

    int curWidth = ss_factor > 1 ? width * ss_factor : width;
    int curHeight = ss_factor > 1 ? height * ss_factor : height;

    // form a square to wrap the triangle to judge the pixels
    // if one vectex is out of screen, compare it with border position
    int minX = std::max(0, (int)std::min({v0.x, v1.x, v2.x}));
    int minY = std::max(0, (int)std::min({v0.y, v1.y, v2.y}));
    int maxX = std::min(curWidth - 1, (int)std::max({v0.x, v1.x, v2.x}));
    int maxY = std::min(curHeight - 1, (int)std::max({v0.y, v1.y, v2.y}));

    float area = EdgeFunction(v0, v1, v2);
    if(area <= 0) return;
    if(std::abs(area) < 1e-6f) return;

    // traverse pixels in the square
    for (int y = minY; y <= maxY; y++)
        for (int x = minX; x <= maxX; x++) {
            Vec2 p(x + 0.5f, y + 0.5f); // pixel origin

            float w0 = EdgeFunction(v1, v2, p);
            float w1 = EdgeFunction(v2, v0, p);
            float w2 = EdgeFunction(v0, v1, p);

            // when p is inside the triangle
            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                float b0 = w0 / area;
                float b1 = w1 / area;
                float b2 = w2 / area;

                // perspective coreection: interpolate 1/w and z/w，then recover z_ndc
                float interp_wrecip = b0 * tri.w_clip[0] + b1 * tri.w_clip[1] + b2 * tri.w_clip[2];
                float interp_zoverw  = b0 * tri.depth_over_w[0] + b1 * tri.depth_over_w[1] + b2 * tri.depth_over_w[2];

                float z_ndc = (interp_wrecip > 1e-6f) ? (interp_zoverw / interp_wrecip) : 1.0f;
                float depth01 = (z_ndc + 1.0f) * 0.5f; // cast to [0,1]

                // correct the color
                unsigned char final_r = tri.r, final_g = tri.g, final_b = tri.b;
                if (interp_wrecip > 1e-6f) {
                    float r_over_w = b0 * (tri.vr[0] * tri.w_clip[0]) + b1 * (tri.vr[1] * tri.w_clip[1]) + b2 * (tri.vr[2] * tri.w_clip[2]);
                    float g_over_w = b0 * (tri.vg[0] * tri.w_clip[0]) + b1 * (tri.vg[1] * tri.w_clip[1]) + b2 * (tri.vg[2] * tri.w_clip[2]);
                    float b_over_w = b0 * (tri.vb[0] * tri.w_clip[0]) + b1 * (tri.vb[1] * tri.w_clip[1]) + b2 * (tri.vb[2] * tri.w_clip[2]);

                    final_r = static_cast<unsigned char>(std::clamp(r_over_w / interp_wrecip, 0.0f, 255.0f));
                    final_g = static_cast<unsigned char>(std::clamp(g_over_w / interp_wrecip, 0.0f, 255.0f));
                    final_b = static_cast<unsigned char>(std::clamp(b_over_w / interp_wrecip, 0.0f, 255.0f));
                }

                SetPixelWithDepth(x, y, depth01, final_r, final_g, final_b);
            }
        }
}

void PPM::RasterizeTriangle(const Triangle &tri) {
    // turn to screen position 
    Vec2 v0 = WorldToScreen(tri.v_2[0]);
    Vec2 v1 = WorldToScreen(tri.v_2[1]);
    Vec2 v2 = WorldToScreen(tri.v_2[2]);

    if (ss_factor > 1) {
        v0.x *= ss_factor;
        v0.y *= ss_factor;
        v1.x *= ss_factor;
        v1.y *= ss_factor;
        v2.x *= ss_factor;
        v2.y *= ss_factor;
    }

    int curWidth = ss_factor > 1 ? width * ss_factor : width;
    int curHeight = ss_factor > 1 ? height * ss_factor : height;

    // form a square to wrap the triangle to judge the pixels
    // if one vectex is out of screen, compare it with border position
    int minX = std::max(0, (int)std::min({v0.x, v1.x, v2.x}));
    int minY = std::max(0, (int)std::min({v0.y, v1.y, v2.y}));
    int maxX = std::min(curWidth - 1, (int)std::max({v0.x, v1.x, v2.x}));
    int maxY = std::min(curHeight - 1, (int)std::max({v0.y, v1.y, v2.y}));

    // traverse pixels in the square
    for (int y = minY; y <= maxY; y++)
        for (int x = minX; x <= maxX; x++) {
            Vec2 p(x + 0.5f, y + 0.5f); // pixel origin

            float w0 = EdgeFunction(v1, v2, p);
            float w1 = EdgeFunction(v2, v0, p);
            float w2 = EdgeFunction(v0, v1, p);

            // when p is inside the triangle
            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                SetPixel(x, y, tri.r, tri.g, tri.b);
            }
        }
}

bool WritePPM(const std::vector<unsigned char> &buf, int w, int h, const std::string &filename) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs)
        return false;

    // ppm header and part of data
    ofs << "P6\n" << w << " " << h << "\n255\n";

    //"reinterpret_cast<const char *>" turns data into const char * pointer which fstream write needs
    ofs.write(reinterpret_cast<const char *>(buf.data()), static_cast<std::streamsize>(buf.size()));

    ofs.close();

    return true;
}

//save SSAA or no-SSAA 's final ppm file
bool PPM::SaveFinalPPM(const std::string &filename) {
    return WritePPM(data, width, height, filename);
}

//save the super sample ppm file
bool PPM::SaveRawPPM(const std::string &filename) {
    return WritePPM(hr_buffer, width * ss_factor, height * ss_factor, filename);
}