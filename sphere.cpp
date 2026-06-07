#include "sphere.h"
#include "ray.h"
#include <cmath>
#include <algorithm>

// bool Sphere::Intersect(const Ray &ray, HitInfo &hit_info) const
// {
//     float a = dot(ray.direction, ray.direction);
//     float b = 2 * dot(ray.direction, ray.origin - position);
//     Vec3 oc = ray.origin - position;
//     float c = dot(oc, oc) - radius * radius;
//     float t1 = 0;
//     float t2 = 0;
//     float t = -1.0f;

//     float disk = pow(b, 2) - 4 * a * c;

//     if (disk < 0)
//     {
//         return false;
//     }

//     t1 = (-b - sqrt(disk)) / (2 * a);
//     if (disk > 0)
//     {
//         t2 = (-b + sqrt(disk)) / (2 * a);
//     }

//     if (t1 > 0)
//     {
//         t = t1;
//     }
//     else if (t1 < 0 && t2 > 0)
//     {
//         t = t2;
//     }

//     if (t1 > ray.min_distance && t1 < ray.max_distance)
//         t = t1;
//     else if (t2 > ray.min_distance && t2 < ray.max_distance)
//         t = t2;
//     else
//         return false;

//     hit_info.t = t;
//     hit_info.point = ray.origin + (ray.direction * t);
//     hit_info.normal = unit_vector(hit_info.point - position);
//     hit_info.material = this->material;

//     return true;

//     return false;
// }

bool Sphere::Intersect(const Ray &ray, HitInfo &hit_info) const
{
    Matrix mInv = this->transformationMatrix.Inverse();

    Vec3 obj_origin = mInv.MultiplyWithVec3(ray.origin);
    Vec3 obj_direction = mInv.MultiplyWithVec3(ray.direction);

    //     float a = dot(ray.direction, ray.direction);
    // float b = 2 * dot(ray.direction, ray.origin - position);

    float a = dot(obj_direction, obj_direction);
    float b = 2 * dot(obj_direction, obj_origin - position);
    Vec3 oc = obj_origin - position;
    float c = dot(oc, oc) - radius * radius;

    float disk = (b * b) - (4 * a * c);
    if (disk < 0)
        return false;

    float sqrt_disk = sqrt(disk);
    float t1 = (-b - sqrt_disk) / (2 * a);
    float t2 = (-b + sqrt_disk) / (2 * a);

    float t = -1.0f;
    if (t1 > ray.min_distance && t1 < ray.max_distance)
        t = t1;
    else if (t2 > ray.min_distance && t2 < ray.max_distance)
        t = t2;
    else
        return false;

    hit_info.t = t;
    hit_info.point = ray.origin + (ray.direction * t);

    // T8
    Vec3 normal_obj = unit_vector((obj_origin + obj_direction * t) - position);

    // hit_info.normal = unit_vector(hit_info.point - position);
    hit_info.normal = unit_vector(mInv.Transpose().MultiplyWithVec3(normal_obj));

    hit_info.material = this->material;
    return true;
}