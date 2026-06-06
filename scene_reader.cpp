#include <iostream>
#include "scene_reader.h"
#include "pugixml.hpp"
#include <string>
#include "scene.h"
#include "ambient_light.h"
#include "point_light.h"
#include "spot_light.h"
#include "parallel_light.h"
#include "solid.h"
#include "texture.h"
#include "types.h"
#include "sphere.h"
#include "mesh.h"
#include <vector>
#include "light.h"
#include "parser.h"

using namespace std;

Scene SceneReader::ReadScene(const string &filename)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (result)
    {
        pugi::xml_node scene = doc.child("scene");
        if (scene)
        {
            Scene scene_data;

            string output_file = scene.attribute("output_file").as_string();

            Vec3 bg_color;
            Camera camera;
            vector<Light *> lights;
            vector<Transform *> transformations;

            vector<Material *> materials;
            Texture texture_material;

            vector<Surface *> surfaces;

            GetBackgroundColor(scene, bg_color);

            GetCamera(scene, camera);

            GetLight(scene, lights);

            GetSurface(scene, surfaces);

            return Scene(bg_color, camera, lights, surfaces);
        }
    }
    std::cout << "File couldn't be read!" << std::endl;

    return Scene{};
}

void SceneReader::GetSurface(pugi::xml_node &scene, std::vector<Surface *> &surfaces)
{
    pugi::xml_node surface_node = scene.child("surfaces");
    if (surface_node)
    {
        for (pugi::xml_node surface_type : surface_node.children())
        {
            Transform transform;
            Matrix object_transform_matrix = Matrix::identity();
            Material *current_material = nullptr;

            GetMaterial(surface_type, current_material);

            GetTransformMatrix(surface_type, transform, object_transform_matrix);
            if (surface_type.name() == string("sphere"))
            {
                float radius = surface_type.attribute("radius").as_float();

                pugi::xml_node pos_node = surface_type.child("position");
                float pos_x = pos_node.attribute("x").as_double();
                float pos_y = pos_node.attribute("y").as_double();
                float pos_z = pos_node.attribute("z").as_double();
                Vec3 positions(pos_x, pos_y, pos_z);

                surfaces.push_back(new Sphere(
                    positions,
                    current_material,
                    object_transform_matrix, radius));
            }
            else if (surface_type.name() == string("mesh"))
            {
                string file_name = surface_type.attribute("name").as_string();
                Mesh *newMesh = new Mesh(file_name, current_material, object_transform_matrix);
                Parser::ParseObjFile(file_name, *newMesh);
                surfaces.push_back(newMesh);
            }
        }
    }
}

void SceneReader::GetMaterial(pugi::xml_node &object, Material *&current_material)
{
    pugi::xml_node solid_material = object.child("material_solid");
    pugi::xml_node textured_material = object.child("material_textured");
    if (solid_material)
    {
        pugi::xml_node color_node = solid_material.child("color");
        float r = color_node.attribute("r").as_float();
        float g = color_node.attribute("g").as_float();
        float b = color_node.attribute("b").as_float();
        Vec3 rgb_color(r, g, b);

        pugi::xml_node phong_node = solid_material.child("phong");
        float ka = phong_node.attribute("ka").as_float();
        float kd = phong_node.attribute("kd").as_float();
        float ks = phong_node.attribute("ks").as_float();
        float exponent = phong_node.attribute("exponent").as_float();

        pugi::xml_node reflectance_node = solid_material.child("reflectance");
        float reflectance_r = reflectance_node.attribute("r").as_float();

        pugi::xml_node transmittance_node = solid_material.child("transmittance");
        float transmittance_t = transmittance_node.attribute("t").as_float();

        pugi::xml_node refraction_node = solid_material.child("refraction");
        float refraction_iof = refraction_node.attribute("iof").as_float();

        current_material = new Solid(
            rgb_color,
            Phong{
                ka,
                kd,
                kd,
                exponent},
            reflectance_r,
            transmittance_t,
            refraction_iof);
    }
    else if (textured_material)
    {
        pugi::xml_node name_node = solid_material.child("texture");
        string name = name_node.attribute("name").as_string();

        pugi::xml_node phong_node = solid_material.child("phong");
        float ka = phong_node.attribute("ka").as_float();
        float kd = phong_node.attribute("kd").as_float();
        float ks = phong_node.attribute("ks").as_float();
        float exponent = phong_node.attribute("exponent").as_float();

        pugi::xml_node reflectance_node = solid_material.child("reflectance");
        float reflectance_r = reflectance_node.attribute("r").as_float();

        pugi::xml_node transmittance_node = solid_material.child("transmittance");
        float transmittance_t = transmittance_node.attribute("t").as_float();

        pugi::xml_node refraction_node = solid_material.child("refraction");
        float refraction_iof = refraction_node.attribute("iof").as_float();

        current_material = new Texture(
            name,
            Phong{
                ka,
                kd,
                kd,
                exponent},
            reflectance_r,
            transmittance_t,
            refraction_iof);
    }
}

void SceneReader::GetLight(pugi::xml_node &scene, std::vector<Light *> &lights)
{
    pugi::xml_node light_node = scene.child("lights");
    if (light_node)
    {
        for (pugi::xml_node light_type : light_node.children())
        {
            pugi::xml_node color_node = light_type.child("color");
            float r = color_node.attribute("r").as_float();
            float g = color_node.attribute("g").as_float();
            float b = color_node.attribute("b").as_float();
            Vec3 rgb_color(r, g, b);

            pugi::xml_node position_node = light_type.child("position");
            float x = position_node.attribute("x").as_float();
            float y = position_node.attribute("y").as_float();
            float z = position_node.attribute("z").as_float();
            Vec3 position(x, y, z);

            pugi::xml_node direction_node = light_type.child("direction");
            float dir_x = direction_node.attribute("x").as_float();
            float dir_y = direction_node.attribute("y").as_float();
            float dir_z = direction_node.attribute("z").as_float();
            Vec3 direction(dir_x, dir_y, dir_z);

            pugi::xml_node falloff_node = light_type.child("falloff");
            int alpha1 = falloff_node.attribute("alpha1").as_int();
            int alpha2 = falloff_node.attribute("alpha2").as_int();

            if (light_type.name() == string("ambient_light"))
            {
                lights.push_back(new AmbientLight(rgb_color));
                // push the pointer of light_type
            }
            else if (light_type.name() == string("point_light"))
            {
                lights.push_back(new PointLight(
                    rgb_color,
                    position));
            }
            else if (light_type.name() == string("spot_light"))
            {
                lights.push_back(new SpotLight(
                    rgb_color,
                    position,
                    direction,
                    Falloff{alpha1, alpha2}));
            }
            else if (light_type.name() == string("parallel_light"))
            {
                lights.push_back(new ParallelLight(
                    rgb_color,
                    direction));
            }
        }
    }
}

void SceneReader::GetCamera(pugi::xml_node &scene, Camera &camera)
{
    pugi::xml_node camera_node = scene.child("camera");
    if (camera_node)
    {
        pugi::xml_node pos_node = camera_node.child("position");
        float pos_x = pos_node.attribute("x").as_double();
        float pos_y = pos_node.attribute("y").as_double();
        float pos_z = pos_node.attribute("z").as_double();
        Vec3 positions(pos_x, pos_y, pos_z);

        pugi::xml_node look_node = camera_node.child("lookat");
        float look_pos_x = look_node.attribute("x").as_double();
        float look_pos_y = look_node.attribute("y").as_double();
        float look_pos_z = look_node.attribute("z").as_double();
        Vec3 looks(look_pos_x, look_pos_y, look_pos_z);

        pugi::xml_node up_node = camera_node.child("up");
        float up_pos_x = up_node.attribute("x").as_double();
        float up_pos_y = up_node.attribute("y").as_double();
        float up_pos_z = up_node.attribute("z").as_double();
        Vec3 ups(up_pos_x, up_pos_y, up_pos_z);

        pugi::xml_node resolution_node = camera_node.child("resolution");

        pugi::xml_node fov_node = camera_node.child("horizontal_fov");
        float fov = fov_node.attribute("angle").as_float();

        camera = Camera(
            positions,
            looks,
            ups,
            fov,
            Resolution{resolution_node.attribute("horizontal").as_int(), resolution_node.attribute("vertical").as_int()},
            camera_node.child("max_bounces").attribute("n").as_int());
    }
}

void SceneReader::GetBackgroundColor(pugi::xml_node &scene, Vec3 &bg_color)
{

    pugi::xml_node background_color = scene.child("background_color");
    if (background_color)
    {
        float r = background_color.attribute("r").as_float();
        float g = background_color.attribute("g").as_float();
        float b = background_color.attribute("b").as_float();

        bg_color = Vec3(r, g, b);
    }
}

void SceneReader::GetTransformMatrix(pugi::xml_node &object, Transform &transform, Matrix &object_transform_matrix)
{
    float radConversion = 3.14159 / 180;
    pugi::xml_node transform_list = object.child("transform");
    if (transform_list)
    {
        for (pugi::xml_node op : transform_list.children())
        {
            if (op.name() == string("translate"))
            {
                Vec3 translate_node = Vec3(op.attribute("x").as_float(), op.attribute("y").as_float(), op.attribute("z").as_float());
                Matrix translate_matrix = transform.makeMatrixForTranslate(translate_node);
                object_transform_matrix = translate_matrix * object_transform_matrix;
            }
            else if (op.name() == string("rotateX"))
            {
                // radious to degree
                float rotateX_node = op.attribute("rotateX").as_float();
                Matrix translate_matrix = transform.makeMatrixForXRotate(rotateX_node * radConversion);
                object_transform_matrix = translate_matrix * object_transform_matrix;
            }
            else if (op.name() == string("rotateY"))
            {
                float rotateY_node = op.attribute("rotateY").as_float();
                Matrix translate_matrix = transform.makeMatrixForYRotate(rotateY_node * radConversion);
                object_transform_matrix = translate_matrix * object_transform_matrix;
            }
            else if (op.name() == string("rotateZ"))
            {
                float rotateZ_node = op.attribute("rotateZ").as_float();
                Matrix translate_matrix = transform.makeMatrixForZRotate(rotateZ_node * radConversion);
                object_transform_matrix = translate_matrix * object_transform_matrix;
            }
            else if (op.name() == string("scale"))
            {
                Vec3 scale_node = Vec3(op.attribute("x").as_float(), op.attribute("y").as_float(), op.attribute("z").as_float());
                Matrix translate_matrix = transform.makeMatrixForScale(scale_node);
                object_transform_matrix = translate_matrix * object_transform_matrix;
            }
        }
    }
}
