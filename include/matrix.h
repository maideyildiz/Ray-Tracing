#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <cmath>
#include <iostream>

class Matrix
{
public:
    float m[4][4];

    Matrix();

    Matrix(float mat[4][4]);

    Matrix operator*(const Matrix &other) const;

    Vec3 multiplywithVec3(const Vec3 &v);
    static Matrix identity();
    Matrix inverse();

    Matrix transpose();
};

#endif