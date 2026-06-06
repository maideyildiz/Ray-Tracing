
#ifndef SCENE_READER_H
#define SCENE_READER_H

#include <string>
#include "pugixml.hpp"
#include "scene.h"

class SceneReader
{
public:
    Scene ReadScene(const std::string &filename);

private:
    void GetSurface(pugi::xml_node &scene, std::vector<Surface *> &surfaces);
    void GetMaterial(pugi::xml_node &object, Material *&current_material);
    void GetLight(pugi::xml_node &scene, std::vector<Light *> &lights);
    void GetCamera(pugi::xml_node &scene, Camera &camera);
    void GetBackgroundColor(pugi::xml_node &scene, Vec3 &bg_color);
    void GetTransformMatrix(pugi::xml_node &object, Transform &transform, Matrix &object_transform_matrix);
};

#endif