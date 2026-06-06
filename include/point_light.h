#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"

class PointLight : public Light
{
public:
    PointLight() = default;
    PointLight(Vec3 color, Vec3 position) : Light(POINT, color, position, Vec3(0, 0, 0), Falloff{0, 0}) {}
};

#endif