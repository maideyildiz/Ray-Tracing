#include "ray.h"

Vec3 Ray::PointInRay(float t) const
{
    return origin + t * direction;
}