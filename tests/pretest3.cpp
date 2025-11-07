#include "PPM.hpp"
#include "Triangle.hpp"

#include <iostream>

int main() {
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const int ss = 4;

    PPM ppm_ssaa(WIDTH, HEIGHT, ss);
    PPM ppm_no_ssaa(WIDTH, HEIGHT, 1);

    ppm_ssaa.BeginFrame();
    ppm_no_ssaa.BeginFrame();

    std::vector<Triangle> triangles;

    triangles.push_back(Triangle(Vec2(0.0f, 0.9f), Vec2(0.0f, 0.0f), Vec2(0.5f, 0.8f), 255, 0, 0));

    for (const auto &tri : triangles) {
        ppm_ssaa.RasterizeTriangle(tri);
        ppm_no_ssaa.RasterizeTriangle(tri);
    }

    ppm_no_ssaa.SaveFinalPPM("output/pretest3/no_ssaa.ppm");

    ppm_ssaa.SaveRawPPM("output/pretest3/raw_ssaa.ppm");

    ppm_ssaa.ResolveSSAA();
    ppm_ssaa.SaveFinalPPM("output/pretest3/final_ssaa.ppm");

    std::cout << "Pretest3 finished: SSAA rasteriztion triangles" << std::endl;

    return 0;
}