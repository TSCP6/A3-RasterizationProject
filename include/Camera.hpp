#pragma once

#include "Matrix.hpp"

class Camera {
  private:
    Vec3 position; // camera position
    Vec3 target;   // view target
    Vec3 up;

    float fov;
    float aspect;
    float near;
    float far;

    Matrix view_matrix;
    Matrix projection_matrix;

    void UpdateViewMatrix();
    void UpdateProjectionMatrix();

  public:
    Camera(const Vec3 &pos = Vec3(0, 0, 5), const Vec3 &tar = Vec3(0, 0, 0), const Vec3 &up = Vec3(0, 1, 0),
           float fov = 3.14159f / 4.0f, // 45 degree
           float aspect = 1.0f, float near = 0.1f, float far = 100.0f);

    Matrix getViewMatrix() const;

    Matrix getProjectionMatrix() const;

    Matrix getViewProjectionMatrix() const;

    void setPosition(const Vec3 &pos);

    void setUp(const Vec3 &upVec);

    void setLookAt(const Vec3 &target);

    void setPerspective(float fov, float aspect, float near, float far);

    // get camera params
    const Vec3 &getPosition() const;
};