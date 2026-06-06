#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "types.h"
class Camera
{
public:
  Vec3 position;
  Vec3 lookat;
  Vec3 up;
  float horizontal_fov;
  Resolution resolution;
  int max_bounces;
  Camera() = default;
  Camera(Vec3 position, Vec3 lookat, Vec3 up, float horizontal_fov, Resolution resolution, int max_bounces)
      : position(position), lookat(lookat), up(up), horizontal_fov(horizontal_fov), resolution(resolution), max_bounces(max_bounces) {};
};

#endif