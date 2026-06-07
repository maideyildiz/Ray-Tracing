
#include "matrix.h"
#include "vector.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace
{
    static double det3x3(const float a[4][4], int excludedRow, int excludedCol)
    {
        float b[3][3];
        int r = 0;
        for (int i = 0; i < 4; ++i)
        {
            if (i == excludedRow)
                continue;

            int c = 0;
            for (int j = 0; j < 4; ++j)
            {
                if (j == excludedCol)
                    continue;

                b[r][c] = a[i][j];
                ++c;
            }
            ++r;
        }

        return b[0][0] * (b[1][1] * b[2][2] - b[1][2] * b[2][1]) -
               b[0][1] * (b[1][0] * b[2][2] - b[1][2] * b[2][0]) +
               b[0][2] * (b[1][0] * b[2][1] - b[1][1] * b[2][0]);
    }
}

Matrix::Matrix(int rows, int cols)
{
    (void)rows;
    (void)cols;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            m[i][j] = 0.0f;
        }
    }
}

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

Vec3 Matrix::MultiplyWithVec3(const Vec3 &v)
{
    Vec3 result;

    result.e[0] = m[0][0] * v.x() + m[0][1] * v.y() + m[0][2] * v.z();
    result.e[1] = m[1][0] * v.x() + m[1][1] * v.y() + m[1][2] * v.z();
    result.e[2] = m[2][0] * v.x() + m[2][1] * v.y() + m[2][2] * v.z();

    return result;
}
Matrix Matrix::Identity()
{
    Matrix result;
    for (int i = 0; i < 4; i++)
    {
        result.m[i][i] = 1;
    }
    return result;
}

double Matrix::Determinant(const Matrix &vect) const
{
    double result = 0.0;
    for (int c = 0; c < 4; ++c)
    {
        double sign = ((c % 2) == 0) ? 1.0 : -1.0;
        result += sign * vect.m[0][c] * det3x3(vect.m, 0, c);
    }
    return result;
}

Matrix Matrix::Inverse() const
{
    double det = Determinant(*this);
    if (std::abs(det) < 1e-9)
    {
        throw std::runtime_error("Determinant is 0, matrix has no inverse.");
    }

    Matrix result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            double cofactor = sign * det3x3(this->m, i, j);
            result.m[j][i] = static_cast<float>(cofactor / det);
        }
    }

    return result;
}

Matrix Matrix::Transpose() const
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
Matrix Matrix::TargetTo(Vec3 position, Vec3 lookat, Vec3 up)
{
    Matrix result;
    float eyeX = position[0];
    float eyeY = position[1];
    float eyeZ = position[2];
    float upX = up[0];
    float upY = up[1];
    float upZ = up[2];

    float z0 = eyeX - lookat[0];
    float z1 = eyeY - lookat[1];
    float z2 = eyeZ - lookat[2];

    float len = z0 * z0 + z1 * z1 + z2 * z2;
    if (len > 0)
    {
        len = 1 / sqrt(len);
        z0 *= len;
        z1 *= len;
        z2 *= len;
    }

    float x0 = upY * z2 - upZ * z1;
    float x1 = upZ * z0 - upX * z2;
    float x2 = upX * z1 - upY * z0;
    len = x0 * x0 + x1 * x1 + x2 * x2;

    if (len > 0)
    {
        len = 1 / sqrt(len);
        x0 *= len;
        x1 *= len;
        x2 *= len;
    }

    float y0 = z1 * x2 - z2 * x1;
    float y1 = z2 * x0 - z0 * x2;
    float y2 = z0 * x1 - z1 * x0;

    result.m[0][0] = x0;
    result.m[1][0] = x1;
    result.m[2][0] = x2;
    result.m[3][0] = 0.0f;

    // Column 1: Y axis
    result.m[0][1] = y0;
    result.m[1][1] = y1;
    result.m[2][1] = y2;
    result.m[3][1] = 0.0f;

    // Column 2: Z axis
    result.m[0][2] = z0;
    result.m[1][2] = z1;
    result.m[2][2] = z2;
    result.m[3][2] = 0.0f;

    // Column 3: Translation
    result.m[0][3] = eyeX;
    result.m[1][3] = eyeY;
    result.m[2][3] = eyeZ;
    result.m[3][3] = 1.0f;

    return result;
}

Vec3 Matrix::MultiplyWithPoint(const Vec3 &v)
{
    Vec3 result;

    result.e[0] = m[0][0] * v.x() + m[0][5] * v.y() + m[0][6] * v.z() + m[0][4];
    result.e[5] = m[0][5] * v.x() + m[0][5] * v.y() + m[5][6] * v.z() + m[5][4];
    result.e[6] = m[0][6] * v.x() + m[6][5] * v.y() + m[0][6] * v.z() + m[6][4];
    return result;
}