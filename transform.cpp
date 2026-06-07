#include "transform.h"

Transform::Transform()
{
    transformationMatrix = Matrix::Identity();
}

Matrix Transform::makeMatrixForTranslate(Vec3 translate)
{
    Matrix matrix = Matrix::Identity();
    matrix.m[0][3] = translate.x();
    matrix.m[1][3] = translate.y();
    matrix.m[2][3] = translate.z();
    return matrix;
}

Matrix Transform::makeMatrixForScale(Vec3 scale)
{
    Matrix matrix = Matrix::Identity();
    matrix.m[0][0] = scale.x();
    matrix.m[1][1] = scale.y();
    matrix.m[2][2] = scale.z();
    return matrix;
}

Matrix Transform::makeMatrixForXRotate(float angle)
{
    Matrix matrix = Matrix::Identity();
    matrix.m[1][1] = cos(angle);
    matrix.m[1][2] = -sin(angle);
    matrix.m[2][1] = sin(angle);
    matrix.m[2][2] = cos(angle);
    return matrix;
}

Matrix Transform::makeMatrixForYRotate(float angle)
{
    Matrix matrix = Matrix::Identity();
    matrix.m[0][0] = cos(angle);
    matrix.m[0][2] = sin(angle);
    matrix.m[2][0] = -sin(angle);
    matrix.m[2][2] = cos(angle);
    return matrix;
}

Matrix Transform::makeMatrixForZRotate(float angle)
{
    Matrix matrix = Matrix::Identity();
    matrix.m[0][0] = cos(angle);
    matrix.m[0][1] = -sin(angle);
    matrix.m[1][0] = sin(angle);
    matrix.m[1][1] = cos(angle);
    return matrix;
}

void Transform::createTransform(Matrix transformation)
{
    transformationMatrix = transformationMatrix * transformation;
}
