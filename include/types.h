#ifndef TYPES_H
#define TYPES_H

#include "vector.h"

class Material;

struct Resolution
{
    int horizontal;
    int vertical;
};

struct Falloff
{
    int alpha1;
    int alpha2;
};

struct Phong
{
    float ambient;
    float diffuse;
    float specular;
    float shininess;
};

struct HitInfo
{
    double t;
    Vec3 point;
    Vec3 normal;
    Material *material;
};

enum LightType
{
    AMBIENT,
    POINT,
    PARALLEL,
    SPOT
};

#endif