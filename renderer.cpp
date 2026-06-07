#include "renderer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <cmath>
#include <cmath>
#include <algorithm>
#include "texture.h"
// #include "matrix.h"
using namespace std;

void Renderer::RenderScene(Scene scene)
{
    unsigned char *data = new unsigned char[scene.camera.resolution.vertical * scene.camera.resolution.horizontal * 3];

    float radConversion = 3.14159 / 180;
    float radianX = scene.camera.horizontal_fov * radConversion;

    float fovY = scene.camera.horizontal_fov * ((float)scene.camera.resolution.vertical / (float)scene.camera.resolution.horizontal);
    float radianY = fovY * radConversion;
    Matrix target = Matrix::TargetTo(scene.camera.position, scene.camera.lookat, scene.camera.up);

    for (int i = 0; i < scene.camera.resolution.vertical; i++)
    {
        for (int j = 0; j < scene.camera.resolution.horizontal; j++)
        {
            float xn = (j + 0.5) / scene.camera.resolution.horizontal;
            float yn = 1.0f - (i + 0.5f) / scene.camera.resolution.vertical; // float yn = 1.0f - (v + 0.5f) / height;
            // float xi = 2 * xn - 1;
            // float yi = 2 * yn - 1;
            float xi = (2 * xn - 1) * tan(radianX);
            float yi = (2 * yn - 1) * tan(radianY);

            Vec3 direction(xi, yi, -1);
            // Vec3 normalized_direction = unit_vector(direction);
            Vec3 normalized_direction = unit_vector(target.MultiplyWithVec3(direction));

            Ray scene_ray(scene.camera.position, normalized_direction, 0.0001f, 999999.0f);
            Vec3 scene_color = Trace(scene, scene_ray, 0);

            int index = (i * scene.camera.resolution.horizontal + j) * 3;
            // data[index] = (unsigned char)(min(1.0, scene_color.e[0]) * 255.0);
            data[index + 0] = (unsigned char)(min(1.0, scene_color.e[0]) * 255.0);
            data[index + 1] = (unsigned char)(min(1.0, scene_color.e[1]) * 255.0);
            data[index + 2] = (unsigned char)(min(1.0, scene_color.e[2]) * 255.0);
        }
    }

    stbi_write_png("rendered_scene.png", scene.camera.resolution.horizontal,
                   scene.camera.resolution.vertical, 3, data,
                   scene.camera.resolution.horizontal * 3);
    delete[] data;
}

Vec3 Renderer::Illuminate(const Ray &ray, const HitInfo &hit, const Light &light)
{
    Vec3 ambient(0, 0, 0);
    Vec3 diffuse(0, 0, 0);
    Vec3 specular(0, 0, 0);

    Vec3 surfaceColor = hit.material->color;

    // T6: Texture Mapping
    if (hit.material->textureName != "")
    {
        Texture *tex = static_cast<Texture *>(hit.material);

        float u = hit.u - floor(hit.u);
        float v = hit.v - floor(hit.v);

        int x = static_cast<int>(u * (tex->width - 1));
        int y = static_cast<int>(v * (tex->height - 1));

        int index = (y * tex->width + x) * 4;
        surfaceColor = Vec3(tex->pixels[index] / 255.0f,
                            tex->pixels[index + 1] / 255.0f,
                            tex->pixels[index + 2] / 255.0f);
    }

    if (light.type == AMBIENT)
    {
        // ambient = light.color * hit.material->color * hit.material->phong.ambient;
        ambient = light.color * surfaceColor * hit.material->phong.ambient;

        return ambient;
    }
    else
    {
        Vec3 light_direction(0, 0, 0);
        if (light.type == PARALLEL)
        {
            light_direction = unit_vector(-light.direction);
        }
        else
        {
            light_direction = unit_vector(light.position - hit.point);
        }
        double n_dot_l = dot(hit.normal, light_direction);
        double factor = std::max(0.0, n_dot_l);
        if (factor > 0)
        {
            // diffuse = hit.material->phong.diffuse * factor * light.color * hit.material->color;
            diffuse = hit.material->phong.diffuse * factor * light.color * surfaceColor;

            // r=2∗(−d∗n)∗n+d
            Vec3 reflected = 2 * (dot(unit_vector(-ray.direction), hit.normal)) * hit.normal + unit_vector(ray.direction);
            float factor_dot = std::max(0.0, dot(reflected, light_direction));
            specular = hit.material->phong.specular * light.color * pow(factor_dot, hit.material->phong.shininess);
        }
    }

    return ambient + diffuse + specular;
}

bool Renderer::Shadow(const HitInfo &hit, Light light, vector<Surface *> surfaces)
{
    Vec3 beginingPoint = hit.point + (hit.normal * 0.0001f);
    // Vec3 beginingPoint = hit.point + (unit_vector(hit.normal) * 0.001);
    Vec3 normalized_direction;
    float max_dist;

    if (light.type == PARALLEL)
    {
        normalized_direction = unit_vector(-light.direction);
        max_dist = 999999.0f;
    }
    else
    {
        Vec3 to_light = light.position - hit.point;
        normalized_direction = unit_vector(to_light);
        max_dist = to_light.length();
    }
    // Ray shadowRay(beginingPoint, normalized_direction, 0.001, max_dist);
    Ray shadow_ray(beginingPoint, normalized_direction, 0.0001f, max_dist);
    // Ray shadow_ray(hit.point + hit.normal * 0.0001f, normalized_direction, 0.001f, max_dist);
    HitInfo shadow_hit;
    for (auto surface : surfaces)
    {
        if (surface->Intersect(shadow_ray, shadow_hit))
        {
            // std::cout << "Golge olusturan nesne bulundu!" << std::endl;
            return true;
        }
    }
    return false;
}

Vec3 Renderer::Trace(Scene scene, const Ray &ray, int depth)
{
    Vec3 color = Vec3(0, 0, 0);
    HitInfo closest_hit;
    closest_hit.t = ray.max_distance;
    bool hit_anything = false;
    bool shadow = false;

    for (auto surface : scene.surfaces)
    {
        HitInfo temp_hit;
        temp_hit.t = ray.max_distance;
        if (surface->Intersect(ray, temp_hit))
        {
            if (temp_hit.t < closest_hit.t)
            {
                closest_hit = temp_hit;
                hit_anything = true;
            }
        }
    }
    if (hit_anything)
    {
        Vec3 pixelColor(0, 0, 0);
        for (auto light : scene.lights)
        {
            // std::cout << "Isik konumu: " << light->position << std::endl;
            shadow = false;
            if (light->type != AMBIENT)
                shadow = Shadow(closest_hit, *light, scene.surfaces);
            if (!shadow)
            {
                pixelColor += Illuminate(ray, closest_hit, *light);
            }
        }

        // T4: Specular Reflection
        Vec3 reflectedColor(0, 0, 0);
        Reflection(depth, scene, closest_hit, ray, reflectedColor);

        // T5
        Vec3 refractedColor(0, 0, 0);
        Refraction(ray, closest_hit, depth, scene, refractedColor);

        return (pixelColor * (1.0f - closest_hit.material->reflectance - closest_hit.material->transmittance)) + reflectedColor + refractedColor;
    }
    else
    {
        return scene.background_color;
    }
}

void Renderer::Refraction(const Ray &ray, HitInfo &closest_hit, int depth, Scene &scene, Vec3 &refractedColor)
{
    float dotProduct = dot(ray.direction, closest_hit.normal);
    Vec3 n;
    float n1;
    float nt;
    float cos_theta1;
    if (dotProduct < 0)
    {
        n1 = 1.0;
        nt = closest_hit.material->refraction;
        n = closest_hit.normal;
        cos_theta1 = -dotProduct;
    }
    else
    {
        n1 = closest_hit.material->refraction;
        nt = 1.0;
        n = -closest_hit.normal;
        cos_theta1 = -dot(ray.direction, n);
    }
    float ratio = n1 / nt;
    float k = 1.0f - ratio * ratio * (1.0f - cos_theta1 * cos_theta1);

    if (k < 0)
    {
        Reflection(depth, scene, closest_hit, ray, refractedColor);
    }
    else
    {
        // t = ratio * (v + n * cos_theta1) - n * sqrt(k) [2]
        Vec3 t = ratio * (ray.direction + n * cos_theta1) - n * sqrt(k);

        // Ray Acne
        Ray refractedRay(closest_hit.point - n * 0.001, unit_vector(t), 0.001, 999.0);
        refractedColor = Trace(scene, refractedRay, depth + 1) * closest_hit.material->transmittance;
    }
}

void Renderer::Reflection(int depth, Scene &scene, HitInfo &closest_hit, const Ray &ray, Vec3 &reflectedColor)
{
    if (depth < scene.camera.max_bounces && closest_hit.material->reflectance > 0)
    {
        // r = 2 * (-d * n) * n + d [3]
        Vec3 r = 2 * dot(-ray.direction, closest_hit.normal) * closest_hit.normal + ray.direction;

        Ray reflectedRay(closest_hit.point + closest_hit.normal * 0.001, unit_vector(r), 0.001, 999.0);

        // reflectedColor = Trace(scene, reflectedRay, depth + 1); // * (1 - closest_hit.material->reflectance);
        reflectedColor = Trace(scene, reflectedRay, depth + 1) * closest_hit.material->reflectance;
    }
}
