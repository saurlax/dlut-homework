#pragma once
#include <graphics.h>

#include <initializer_list>

#include "Camera.h"
#include "Math.h"

#define BUFFER_SIZE 800 * 600

class Texture {
  static int count;
  IMAGE image;

 public:
  static Texture byId[];
  static int Create(LPCTSTR path);

  DWORD operator()(int x, int y);
};

class Face {
 public:
  char textureId;
  Vec3 vertices[4];

 public:
  Face();
  Face(std::initializer_list<Vec3> vertices);
};

class Shader {
  Face FAO[BUFFER_SIZE];
  float* zBuffer;
  int count;
  int width;
  int height;

 public:
  DWORD* surface;
  void SetSize(int width, int height);
  void PushBuffer(std::initializer_list<Face> values);
  void ClearBuffer();
  void Draw(Camera& camera);
};
