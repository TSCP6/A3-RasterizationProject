#include "PPM.hpp"

#include <algorithm>
#include <fstream>

PPM::PPM(int w, int h, int ss) : width(w), height(h), ss_factor(ss) {
    // resize function's second parameter fills the empty array members
    data.resize(w * h * 3, 255);
    hr_buffer.resize((w * ss) * (h * ss) * 3, 255);
}

void PPM::BeginFrame() {
    std::fill(data.begin(), data.end(), static_cast<unsigned char>(255));
    std::fill(hr_buffer.begin(), hr_buffer.end(), static_cast<unsigned char>(255));
}

void PPM::EndFrame() {
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

void PPM::RasterizeTriangle(const Triangle &tri) {
    // turn to screen position
    Vec2 v0 = WorldToScreen(tri.v[0]);
    Vec2 v1 = WorldToScreen(tri.v[1]);
    Vec2 v2 = WorldToScreen(tri.v[2]);

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

bool PPM::SaveFinalPPM(const std::string &filename) {
    return WritePPM(data, width, height, filename);
}

bool PPM::SaveRawPPM(const std::string &filename) {
    return WritePPM(hr_buffer, width * ss_factor, height * ss_factor, filename);
}