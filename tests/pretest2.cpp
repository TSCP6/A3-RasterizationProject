#include "PPM.hpp"
#include "Triangle.hpp"

#include <iostream>

int main() {
    const int WIDTH = 800;
    const int HEIGHT = 600;

    PPM image(WIDTH, HEIGHT, 1); // call the constructor, init width and height

    std::vector<Triangle> triangles;

    triangles.push_back(Triangle(Vec2(0.0f, 0.5f), Vec2(0.0f, 0.0f), Vec2(0.2f, 0.3f), 255, 0, 0));
    triangles.push_back(Triangle(Vec2(0.6f, 0.5f), Vec2(0.7f, 0.4f), Vec2(0.8f, 0.5f), 0, 0, 255));
    triangles.push_back(Triangle(Vec2(0.3f, 0.8f), Vec2(0.0f, 0.9f), Vec2(0.2f, 0.7f), 0, 255, 0));
    triangles.push_back(Triangle(Vec2(0.3f, 0.6f), Vec2(0.0f, 0.2f), Vec2(0.2f, 0.1f), 0, 255, 255));

    for (const auto &tri : triangles) {
        image.RasterizeTriangle(tri);
    }

    image.SaveFinalPPM("output/pretest2/pretest2.ppm");

    std::cout << "Pretest2 finished: rasteriztion triangles" << std::endl;

    return 0;
}