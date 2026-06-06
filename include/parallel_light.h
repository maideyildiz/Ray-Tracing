#ifndef PARALLELLIGHT_H
#define PARALLELLIGHT_H

#include "light.h"

class ParallelLight : public Light
{
public:
    ParallelLight() = default;
    ParallelLight(Vec3 color, Vec3 direction) : Light(PARALLEL, color, Vec3(0, 0, 0), direction, Falloff{0, 0}) {}
};

#endif