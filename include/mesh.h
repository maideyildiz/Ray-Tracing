#ifndef MESH_H
#define MESH_H

#include "surface.h"
#include "texture.h"
#include <string>
#include <vector>
#include "vector.h"

class Mesh : public Surface
{
public:
    std::string file_name;
    std::vector<Vec3> vertices;
    std::vector<int> face_indices;
    Mesh() = default;
    Mesh(std::string file_name, Material *texture, Matrix transformationMatrix) : Surface(texture, transformationMatrix), file_name(file_name) {}
    bool Intersect(const Ray &ray, HitInfo &hit_info) const override;
};

#endif