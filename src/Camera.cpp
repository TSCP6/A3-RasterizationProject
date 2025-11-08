#include "Camera.hpp"
#include <cmath>

Camera::Camera(const Vec3 &pos, const Vec3 &tar, const Vec3 &up,
               float fov, float aspect, float near, float far)
    : position(pos), target(tar), up(up), fov(fov), aspect(aspect), near(near), far(far) {
}

Matrix Camera::getViewMatrix() const {
    return Matrix::lookAt(position, target, up);
}

Matrix Camera::getProjectionMatrix() const {
    return Matrix::perspective(fov, aspect, near, far);
}

void Camera::setLookAt(const Vec3 &tar) {
    target = tar;
    view_matrix = Matrix::lookAt(position, target, up);
}

void Camera::setPerspective(float fov, float aspect, float near, float far) {
    this->fov = fov;
    this->aspect = aspect;
    this->near = near;
    this->far = far;
    projection_matrix = Matrix::perspective(fov, aspect, near, far);
}

void Camera::setPosition(const Vec3 &pos) {
    position = pos;
}

void Camera::setUp(const Vec3 &upVec) {
    up = upVec;
}

// get camera params
const Vec3 &Camera::getPosition() const {
    return position;
}
