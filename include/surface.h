#ifndef SURFACE_H
#define SURFACE_H

#include "transform.h"
#include "material.h"
#include "ray.h"

class Surface
{
public:
    Material *material;
    Matrix transformationMatrix;
    Surface() = default;
    Surface(Material *material, Matrix transformationMatrix) : material(material), transformationMatrix(transformationMatrix) {}
    virtual bool Intersect(const Ray &ray, HitInfo &hit_info) const = 0;
};

#endif