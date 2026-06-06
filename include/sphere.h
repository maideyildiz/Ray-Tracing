#ifndef SPHERE_H
#define SPHERE_H

#include "surface.h"
#include "solid.h"
#include "ray.h"

class Sphere : public Surface
{
public:
    Vec3 position;
    float radius;
    Sphere() = default;
    Sphere(Vec3 position, Material *solid, Matrix transformationMatrix, float radius) : Surface(solid, transformationMatrix), position(position), radius(radius) {}
    bool Intersect(const Ray &ray, HitInfo &hit_info) const override;

private:
    void CalculateHitInfo(float t, const Ray &ray, HitInfo &hit_info) const;
};

#endif