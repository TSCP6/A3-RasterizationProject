#include "Camera.hpp"
#include <cmath>

// 移除默认参数（已在头文件中定义）
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

Matrix Camera::getViewProjectionMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}

void Camera::setLookAt(const Vec3 &tar) {
    target = tar;
    UpdateViewMatrix();
}

void Camera::setPosition(const Vec3 &pos) {
    position = pos;
}

void Camera::setPerspective(float fov, float aspect, float near, float far) {
    this->fov = fov;
    this->aspect = aspect;
    this->near = near;
    this->far = far;
    UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix() {
    // 构建LookAt矩阵
    Vec3 zaxis = (position - target).normalize(); // 相机朝向（反向）
    Vec3 xaxis = up.cross(zaxis).normalize();     // 右方向
    Vec3 yaxis = zaxis.cross(xaxis);              // 上方向

    const float m[4][4] = {{xaxis.x, xaxis.y, xaxis.z, -xaxis.dot(position)},
                           {yaxis.x, yaxis.y, yaxis.z, -yaxis.dot(position)},
                           {zaxis.x, zaxis.y, zaxis.z, -zaxis.dot(position)},
                           {0, 0, 0, 1}};

    // View矩阵 = 旋转 × 平移
    view_matrix = Matrix(m);
}

void Camera::UpdateProjectionMatrix() {
    // 透视投影矩阵
    float tanHalfFov = tan(fov * 0.5f * 3.14159265f / 180.0f);
    float range = far - near;

    const float m[4][4] = {
        1.0f / (aspect * tanHalfFov), 0, 0, 0,  0, 1.0f / tanHalfFov, 0, 0, 0, 0, -(far + near) / range,
        -2.0f * far * near / range,   0, 0, -1, 0};
    projection_matrix = Matrix(m);
}

void Camera::setUp(const Vec3 &upVec) {
    up = upVec;
}

// get camera params
const Vec3 &Camera::getPosition() const {
    return position;
}
