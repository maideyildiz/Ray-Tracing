#ifndef TEXTURE_H
#define TEXTURE_H

#include "material.h"
#include <string>

class Texture : public Material
{
public:
    Texture() = default;
    Texture(std::string textureName, Phong phong, float reflectance, float transmittance, float refraction)
        : Material(textureName, Vec3(0, 0, 0), phong, reflectance, transmittance, refraction) {};
};

#endif