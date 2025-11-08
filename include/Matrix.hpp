#pragma once

#include "Vec4.hpp"

// matrix class
class Matrix {
  public:
    float mat[4][4];

  public:
    Matrix();
    Matrix(const float m[4][4]);

  public:
    Matrix operator*(const Matrix &other) const;

    Vec4 operator*(const Vec4 &v4) const;

    Matrix &operator=(const Matrix &other);

    // static factory method to create matrix
    static Matrix translation(float x, float y, float z);
    static Matrix rotationX(float angle);
    static Matrix rotationY(float angle);
    static Matrix rotationZ(float angle);
    static Matrix scale(float sx, float sy, float sz);

    // view matrix
    static Matrix lookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up);

    // perspective matrix
    static Matrix perspective(float fovy, float aspect, float near, float far);

  private:
    static void matrixMultiply(const Matrix &mat1, const Matrix &mat2, Matrix &res);
};