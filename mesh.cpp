#include "mesh.h"
#include "vector"

bool Mesh::Intersect(const Ray &ray, HitInfo &hit_info) const
{
    Matrix mInv = this->transformationMatrix.Inverse();
    Ray obj_ray(mInv.MultiplyWithPoint(ray.origin),
                mInv.MultiplyWithVec3(ray.direction),
                ray.min_distance, ray.max_distance);

    bool hit_anything = false;
    HitInfo temp_hit;

    float closest_t = hit_info.t;

    for (int i = 0; i < face_indices.size(); i += 3)
    {
        int idx1 = uv_indices[i];
        int idx2 = uv_indices[i + 1];
        int idx3 = uv_indices[i + 2];

        if (TriangleIntersect(obj_ray,
                              vertices[face_indices[i]],
                              vertices[face_indices[i + 1]],
                              vertices[face_indices[i + 2]],
                              u_coords[idx1], v_coords[idx1],
                              u_coords[idx2], v_coords[idx2],
                              u_coords[idx3], v_coords[idx3],
                              temp_hit))
        {
            if (temp_hit.t < closest_t)
            {
                closest_t = temp_hit.t;
                hit_info = temp_hit;
                hit_anything = true;
            }
        }
    }

    if (hit_anything)
    {
        hit_info.t = closest_t;
        hit_info.point = ray.origin + (ray.direction * hit_info.t);

        Matrix invTranspose = mInv.Transpose();
        hit_info.normal = unit_vector(invTranspose.MultiplyWithVec3(hit_info.normal));

        if (dot(hit_info.normal, ray.direction) > 0)
            hit_info.normal = -hit_info.normal;

        hit_info.material = this->material;
    }

    return hit_anything;
}

bool Mesh::TriangleIntersect(const Ray &ray, const Vec3 &v1, const Vec3 &v2, const Vec3 &v3, float u1, float v1_uv, float u2, float v2_uv, float u3, float v3_uv, HitInfo &hit) const
{
    Vec3 e1 = v2 - v1;
    Vec3 e2 = v3 - v1;
    Vec3 s = ray.origin - v1;
    Vec3 d_cross_e2 = cross(ray.direction, e2);
    float det = dot(d_cross_e2, e1);

    if (abs(det) < 1e-8)
        return false;

    float inv_det = 1.0f / det;
    float a = dot(d_cross_e2, s) * inv_det;
    if (a < 0.0 || a > 1.0)
        return false;

    Vec3 s_cross_e1 = cross(s, e1);
    float b = dot(s_cross_e1, ray.direction) * inv_det;
    if (b < 0.0 || a + b > 1.0)
        return false;

    float t = dot(s_cross_e1, e2) * inv_det;

    if (t > ray.min_distance && t < ray.max_distance)
    {
        hit.t = t;
        hit.normal = unit_vector(cross(e1, e2));

        float w = 1.0f - a - b;
        hit.u = w * u1 + a * u2 + b * u3;
        hit.v = w * v1_uv + a * v2_uv + b * v3_uv;

        return true;
    }
    return false;
}