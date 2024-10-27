#include "Camera.h"

#include <math.h>

#include "Math.h"

Mat4 Camera::GetViewMatrix() const {
  Vec3 forward(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
  Vec3 right = forward.cross(Vec3(0.0f, 1.0f, 0.0f)).normalize();
  Vec3 up = right.cross(forward).normalize();

  return Mat4::lookAt(position, position + forward, up);
}

Mat4 Camera::GetProjectionMatrix() const {
  return Mat4::perspective(fov, aspectRatio, 0.1f, viewDistance);
}