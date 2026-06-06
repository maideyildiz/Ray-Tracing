#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include "types.h"

class Light
{
public:
  Vec3 color;
  Vec3 position;
  Vec3 direction;
  Falloff falloff;
  LightType type;
  Light() = default;
  Light(LightType type, Vec3 color, Vec3 position, Vec3 direction, Falloff falloff)
      : type(type), color(color), position(position), direction(direction), falloff(falloff) {}
};

#endif