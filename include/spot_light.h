#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "light.h"

class SpotLight : public Light
{
public:
    SpotLight() = default;
    SpotLight(Vec3 color, Vec3 position, Vec3 direction, Falloff falloff) : Light(SPOT, color, position, direction, falloff) {}
};

#endif