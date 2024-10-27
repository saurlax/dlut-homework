#pragma once
#include "Math.h"

class Camera {
 public:
  Vec3 position;
  float aspectRatio;
  float viewDistance = 100.0f;
  float yaw;
  float pitch;
  float fov = 40.0f;

  Mat4 GetViewMatrix() const;
  Mat4 GetProjectionMatrix() const;
};