#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "vector.h"
#include "types.h"

class Material
{
public:
    std::string textureName;
    Vec3 color;
    Phong phong;
    float reflectance;
    float transmittance;
    float refraction;
    Material() = default;
    Material(std::string textureName, Vec3 color, Phong phong, float reflectance, float transmittance, float refraction)
        : textureName(textureName), color(color), phong(phong), reflectance(reflectance), transmittance(transmittance), refraction(refraction) {};
    // Vec3 shade(const Ray &ray, const Intersection &hit, const Light &light)
};

#endif