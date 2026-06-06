#include "sphere.h"
#include "ray.h"
#include <cmath>
#include <algorithm>

bool Sphere::Intersect(const Ray &ray, HitInfo &hit_info) const
{
    float a = dot(ray.direction, ray.direction);
    float b = 2 * dot(ray.direction, ray.origin - position);
    Vec3 oc = ray.origin - position;
    float c = dot(oc, oc) - radius * radius;
    float t1 = 0;
    float t2 = 0;
    float t = -1.0f;

    float disk = pow(b, 2) - 4 * a * c;

    if (disk >= 0)
    {
        t1 = (-b - sqrt(disk)) / (2 * a);
        if (disk > 0)
        {
            t2 = (-b + sqrt(disk)) / (2 * a);
        }

        if (t1 > 0)
        {
            t = t1;
        }
        else if (t1 < 0 && t2 > 0)
        {
            t = t2;
        }

        if (t < ray.max_distance && t > ray.min_distance)
        {
            hit_info.t = t;
            hit_info.point = ray.origin + (t * ray.direction);
            Vec3 normal = ray.PointInRay(t) - position;
            normal = unit_vector(normal);
            hit_info.normal = normal;
            hit_info.material = this->material;
        }
        else
        {
            return false;
        }

        return true;
    }

    return false;
}
