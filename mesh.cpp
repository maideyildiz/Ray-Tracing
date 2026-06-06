#include "mesh.h"

bool Mesh::Intersect(const Ray &ray, HitInfo &hit_info) const
{
    bool hit_anything = false;
    double closest_so_far = hit_info.t;
    for (int i = 0; i < face_indices.size(); i += 3)
    {
        Vec3 v1 = vertices[face_indices[i]];
        Vec3 v2 = vertices[face_indices[i + 1]];
        Vec3 v3 = vertices[face_indices[i + 2]];

        Vec3 e1 = v2 - v1;
        Vec3 e2 = v3 - v1;
        Vec3 s = ray.origin - v1;

        Vec3 d_cross_e2 = cross(ray.direction, e2);
        float det = dot(d_cross_e2, e1);

        if (abs(det) < 1e-8)
            continue;

        float inv_det = 1.0f / det;
        float a = dot(d_cross_e2, s) * inv_det;
        if (a < 0.0 || a > 1.0)
            continue;

        Vec3 s_cross_e1 = cross(s, e1);
        float b = dot(s_cross_e1, ray.direction) * inv_det;
        if (b < 0.0 || a + b > 1.0)
            continue;

        float t = dot(s_cross_e1, e2) * inv_det;

        if (t > ray.min_distance && t < closest_so_far)
        {
            closest_so_far = t;
            hit_info.t = t;
            hit_info.point = ray.origin + t * ray.direction;
            hit_info.normal = unit_vector(cross(e1, e2));
            hit_info.material = this->material;
            hit_anything = true;
        }
    }
    return hit_anything;
}