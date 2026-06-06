#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "types.h"
#include "light.h"
#include "surface.h"
#include <vector>

class Scene
{
public:
    Vec3 background_color;
    Camera camera;
    std::vector<Light *> lights;
    std::vector<Surface *> surfaces;
    Scene() = default;
    Scene(Vec3 background_color, Camera camera, std::vector<Light *> lights, std::vector<Surface *> surfaces)
        : background_color(background_color), camera(camera), lights(lights), surfaces(surfaces) {}
};

#endif