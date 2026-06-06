#include "parser.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include "mesh.h"

void Parser::ParseObjFile(const std::string &obj_file_name, Mesh &mesh)
{
    std::string line;
    std::ifstream obj_file(obj_file_name);
    while (std::getline(obj_file, line))
    {
        if (line == "" || line[0] == '#')
        {
            continue;
        }
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v")
        {
            float x, y, z;
            if (ss >> x >> y >> z)
            {
                mesh.vertices.push_back(Vec3(x, y, z));
            }
        }
        else if (type == "f")
        {
            std::string vertex_data;
            for (int i = 0; i < 3; ++i)
            {
                ss >> vertex_data;
                std::stringstream ss_face(vertex_data);
                std::string index_str;

                std::getline(ss_face, index_str, '/');

                int index = std::stoi(index_str);
                mesh.face_indices.push_back(index > 0 ? index - 1 : 0);
            }
        }
    }
}