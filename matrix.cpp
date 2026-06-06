
#include "matrix.h"
#include "vector.h"
#include <cmath>
#include <iostream>

Matrix::Matrix()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = 0;
        }
    }
}

Matrix::Matrix(float mat[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = mat[i][j];
        }
    }
}

Matrix Matrix::operator*(const Matrix &other) const
{
    Matrix result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j] + m[i][3] * other.m[3][j];
        }
    }
    return result;
}

Vec3 Matrix::multiplywithVec3(const Vec3 &v)
{
    Vec3 result;
    if (v.length() == 0)
    {
        return result;
    }
    result.e[0] = m[0][0] * v.x() + m[0][1] * v.y() + m[0][2] * v.z() + m[0][3];
    result.e[1] = m[1][0] * v.x() + m[1][1] * v.y() + m[1][2] * v.z() + m[1][3];
    result.e[2] = m[2][0] * v.x() + m[2][1] * v.y() + m[2][2] * v.z() + m[2][3];
    return result;
}
Matrix Matrix::identity()
{
    Matrix result;
    for (int i = 0; i < 4; i++)
    {
        result.m[i][i] = 1;
    }
    return result;
}
Matrix Matrix::inverse()
{
    Matrix result;
    // calculate the inverse of the matrix

    return result;
}

Matrix Matrix::transpose()
{
    Matrix result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = m[j][i];
        }
    }
    return result;
}
