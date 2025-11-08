#include "Matrix.hpp"

#include <cmath>

Matrix::Matrix() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

Matrix::Matrix(const float m[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = m[i][j];
        }
    }
}

Matrix Matrix::operator*(const Matrix &other) const {
    Matrix result;
    matrixMultiply(*this, other, result);
    return result;
}

Vec4 Matrix::operator*(const Vec4 &v) const {
    return Vec4(mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3] * v.w,
                mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3] * v.w,
                mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3] * v.w,
                mat[3][0] * v.x + mat[3][1] * v.y + mat[3][2] * v.z + mat[3][3] * v.w);
}

Matrix &Matrix::operator=(const Matrix &other) {
    if (this != &other) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                mat[i][j] = other.mat[i][j];
            }
        }
    }
    return *this;
}

void Matrix::matrixMultiply(const Matrix &mat1, const Matrix &mat2, Matrix &res) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.mat[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                res.mat[i][j] += mat1.mat[i][k] * mat2.mat[k][j];
            }
        }
    }
};

Matrix Matrix::translation(float x, float y, float z) {
    Matrix m;
    m.mat[0][3] = x;
    m.mat[1][3] = y;
    m.mat[2][3] = z;
    return m;
}

Matrix Matrix::rotationX(float angle) {
    Matrix m;
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    m.mat[1][1] = cos;
    m.mat[1][2] = -sin;
    m.mat[2][1] = sin;
    m.mat[2][2] = cos;

    return m;
}

Matrix Matrix::rotationY(float angle) {
    Matrix m;
    float c = std::cos(angle);
    float s = std::sin(angle);

    m.mat[0][0] = c;
    m.mat[0][2] = s;
    m.mat[2][0] = -s;
    m.mat[2][2] = c;

    return m;
}

Matrix Matrix::rotationZ(float angle) {
    Matrix m;
    float c = std::cos(angle);
    float s = std::sin(angle);

    m.mat[0][0] = c;
    m.mat[0][1] = -s;
    m.mat[1][0] = s;
    m.mat[1][1] = c;

    return m;
}

Matrix Matrix::scale(float sx, float sy, float sz) {
    Matrix m;
    m.mat[0][0] = sx;
    m.mat[1][1] = sy;
    m.mat[2][2] = sz;
    return m;
}

// reutrns a matrix that translate and rotate the world space to camera space
Matrix Matrix::lookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up) {
    // the opposite direction of camera's
    Vec3 zAxis = (eye - target).normalize();

    // the right direction
    Vec3 xAxis = up.cross(zAxis).normalize();

    // the up direction
    Vec3 yAxis = zAxis.cross(xAxis);

    Matrix m;

    // rotation area, the transpose(inverse) of rotation matrix since we transform from world to camera
    m.mat[0][0] = xAxis.x;
    m.mat[0][1] = xAxis.y;
    m.mat[0][2] = xAxis.z;

    m.mat[1][0] = yAxis.x;
    m.mat[1][1] = yAxis.y;
    m.mat[1][2] = yAxis.z;

    m.mat[2][0] = zAxis.x;
    m.mat[2][1] = zAxis.y;
    m.mat[2][2] = zAxis.z;

    // translation area
    m.mat[0][3] = -xAxis.dot(eye);
    m.mat[1][3] = -yAxis.dot(eye);
    m.mat[2][3] = -zAxis.dot(eye);

    return m;
}

// returns a matrix to transform the 3d position into clip space to make perspective effects
Matrix Matrix::perspective(float fovy,   // field of view y, the vertical view angle
                           float aspect, // screen width/height
                           float near, float far //z distance of screen camera
                        ) {
    Matrix m;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m.mat[i][j] = 0.0f;

    float tanHalfFovy = std::tan(fovy / 2.0f); //the half angle tangent

    //x,y axis zooming, different zoom makes the real size not be stretch or compress
    m.mat[0][0] = 1.0f / (aspect * tanHalfFovy);
    m.mat[1][1] = 1.0f / tanHalfFovy;

    //zoom
    m.mat[2][2] = -(far + near) / (far - near);

    //tranlate
    m.mat[2][3] = -(2.0f * far *near) / (far - near);

    m.mat[3][2] = -1.0f; //makes the object z positive to compare the depth

    m.mat[3][3] = 0.0f;

    return m;
}