#ifndef SOLID_H
#define SOLID_H

#include "material.h"
#include <string>

class Solid : public Material
{
public:
    Solid() = default;
    Solid(Vec3 color, Phong phong, float reflectance, float transmittance, float refraction)
        : Material("", color, phong, reflectance, transmittance, refraction) {};
};

#endif