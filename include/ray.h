#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Vec3 origin;
    Vec3 direction;
    float max_distance;
    float min_distance;
    Ray(Vec3 origin, Vec3 direction, float min_distance, float max_distance) : origin(origin), direction(direction), min_distance(min_distance), max_distance(max_distance) {}
    Vec3 PointInRay(float t) const;
};

#endif