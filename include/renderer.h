#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "scene.h"
#include "surface.h"

class Renderer
{
public:
    Renderer() = default;
    void RenderScene(Scene scene);

private:
    Vec3 Illuminate(const Ray &ray, const HitInfo &hit, const Light &light);
    bool Shadow(const HitInfo &hit, Light light, std::vector<Surface *> surfaces);
    Vec3 Trace(Scene scene, const Ray &ray, int depth);
    void Refraction(const Ray &ray, HitInfo &closest_hit, int depth, Scene &scene, Vec3 &refractedColor);
    void Reflection(int depth, Scene &scene, HitInfo &closest_hit, const Ray &ray, Vec3 &reflectedColor);
};

#endif