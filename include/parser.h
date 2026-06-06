#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include "vector.h"
#include "mesh.h"

class Parser
{
public:
    static void ParseObjFile(const std::string &obj_file_name, Mesh &mesh); // std::vector<Vec3> &vertices, std::vector<int> &face_indices);
};

#endif