#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vector.h"
#include "matrix.h"

class Transform
{
public:
    Matrix transformationMatrix;
    Transform();
    Matrix makeMatrixForTranslate(Vec3 translate);
    Matrix makeMatrixForScale(Vec3 scale);
    Matrix makeMatrixForXRotate(float angle);
    Matrix makeMatrixForYRotate(float angle);
    Matrix makeMatrixForZRotate(float angle);
    void createTransform(Matrix transformation);
};

#endif