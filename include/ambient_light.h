
#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "light.h"

class AmbientLight : public Light
{
public:
    AmbientLight(Vec3 color) : Light(AMBIENT, color, Vec3(0, 0, 0), Vec3(0, 0, 0), Falloff{0, 0}) {}
};

#endif