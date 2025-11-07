#pragma once
#include "Triangle.hpp"
#include "Vec2.hpp"

#include <string>
#include <vector>

class PPM {
  private:
    int width, height;
    int ss_factor;
    std::vector<unsigned char> data;
    std::vector<unsigned char> hr_buffer;

  public:
    PPM(int w, int h, int ss);

    void BeginFrame(); //clear, prepare the buffer
    void EndFrame(); //operations in the end of rendering

    //transform high-res data into normal data
    void ResolveSSAA();

    bool SaveFinalPPM(const std::string &filename);
    bool SaveRawPPM(const std::string &filename);

    void SetPixelHighRes(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void SetPixelLowRes(int x, int y, unsigned char r, unsigned char g, unsigned char b);

    void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    Vec2 WorldToScreen(const Vec2 &world);
    float EdgeFunction(const Vec2 &v0, const Vec2 &v1, const Vec2 &v2);
    void RasterizeTriangle(const Triangle &tri);
};