#include "Render.hpp"

#include <iostream>

int main() {
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const int SS_FACTOR = 2;

    std::cout << "Rendering Sample 1: Gradient Cube..." << std::endl;
    {
        PPM ppm(WIDTH, HEIGHT, SS_FACTOR);
        Render renderer(WIDTH, HEIGHT);
        Camera camera;
        
        camera.setPosition(Vec3(3.0f, 2.0f, 6.0f));
        camera.setLookAt(Vec3(0.0f, 0.0f, 0.0f));
        camera.setUp(Vec3(0.0f, 1.0f, 0.0f));
        camera.setPerspective(45.0f, (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        
        MeshCube cube;
        cube.SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        cube.SetRotation(Vec3(15.0f, 0.0f, 0.0f));
        cube.SetScale(Vec3(1.0f, 1.0f, 1.0f));
        cube.SetColorMode(0);
        
        ppm.BeginFrame();
        renderer.RenderMesh(cube, camera, ppm);
        ppm.ResolveSSAA();
        ppm.SaveFinalPPM("output/sample1.ppm");
        
        std::cout << "Sample 1 saved to output/sample1.ppm" << std::endl;
    }

    std::cout << "Rendering Sample 2: Checkered Cube..." << std::endl;
    {
        PPM ppm(WIDTH, HEIGHT, SS_FACTOR);
        Render renderer(WIDTH, HEIGHT);
        Camera camera;
        
        camera.setPosition(Vec3(3.0f, 2.0f, 6.0f));
        camera.setLookAt(Vec3(0.0f, 0.0f, 0.0f));
        camera.setUp(Vec3(0.0f, 1.0f, 0.0f));
        camera.setPerspective(45.0f, (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        
        MeshCube cube;
        cube.SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        cube.SetRotation(Vec3(15.0f, 0.0f, 0.0f));
        cube.SetScale(Vec3(1.0f, 1.0f, 1.0f));
        cube.SetColorMode(1);
        
        ppm.BeginFrame();
        renderer.RenderMesh(cube, camera, ppm);
        ppm.ResolveSSAA();
        ppm.SaveFinalPPM("output/sample2.ppm");
        
        std::cout << "Sample 2 saved to output/sample2.ppm" << std::endl;
    }

    std::cout << "Rendering Sample 3: Two Intersecting Cubes..." << std::endl;
    {
        PPM ppm(WIDTH, HEIGHT, SS_FACTOR);
        Render renderer(WIDTH, HEIGHT);
        Camera camera;
        
        camera.setPosition(Vec3(4.0f, 3.0f, 6.0f));
        camera.setLookAt(Vec3(0.0f, 0.0f, 0.0f));
        camera.setUp(Vec3(0.0f, 1.0f, 0.0f));
        camera.setPerspective(45.0f, (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        
        MeshCube cube1;
        cube1.SetPosition(Vec3(-0.7f, 0.3f, -0.5f));
        cube1.SetRotation(Vec3(20.0f, 45.0f, 10.0f));
        cube1.SetScale(Vec3(1.2f, 1.2f, 1.2f));
        cube1.SetColorMode(1); 
        
        MeshCube cube2;
        cube2.SetPosition(Vec3(-0.7f, 0.3f, -0.5f));
        cube2.SetRotation(Vec3(175.0f, 60.0f, 5.0f));
        cube2.SetScale(Vec3(1.2f, 1.2f, 1.2f));
        cube2.SetColorMode(1);
        
        ppm.BeginFrame();
        renderer.RenderMesh(cube1, camera, ppm);
        renderer.RenderMesh(cube2, camera, ppm);
        ppm.ResolveSSAA();
        ppm.SaveFinalPPM("output/sample3.ppm");
        
        std::cout << "Sample 3 saved to output/sample3.ppm" << std::endl;
    }

    std::cout << "\n=== All samples rendered successfully! ===" << std::endl;
    
    return 0;
}