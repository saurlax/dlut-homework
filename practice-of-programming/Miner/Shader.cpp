#include "Shader.h"

#include <stdio.h>

#include "Camera.h"
#include "Math.h"

Texture Texture::byId[256];
int Texture::count = 0;

int Texture::Create(LPCTSTR path) {
  Texture* texture = &byId[count++];
  loadimage(&texture->image, path);
  return count - 1;
}

DWORD Texture::operator()(int x, int y) {
  DWORD* buffer = GetImageBuffer(&image);
  x = x % 16;
  y = y % 16;
  return buffer[y * 16 + x];
}

Face::Face() {}

Face::Face(std::initializer_list<Vec3> vertices) {
  for (int i = 0; i < 4; i++) {
    this->vertices[i] = *(vertices.begin() + i);
  }
}

void Shader::SetSize(int width, int height) {
  this->width = width;
  this->height = height;
  if (zBuffer != nullptr) delete[] zBuffer;
  zBuffer = new float[width * height];
}

void Shader::PushBuffer(std::initializer_list<Face> values) {
  for (Face value : values) {
    FAO[count++] = value;
  }
}

void Shader::ClearBuffer() { count = 0; }

void Shader::Draw(Camera& camera) {
  Mat4 view = camera.GetViewMatrix();
  Mat4 projection = camera.GetProjectionMatrix();
  for (int i = 0; i < count; i++) {
    Vec3 a = (projection * view * Vec4(FAO[i].vertices[0], 1.0f))
                 .perspectiveDivide();
    Vec3 b = (projection * view * Vec4(FAO[i].vertices[1], 1.0f))
                 .perspectiveDivide();
    Vec3 c = (projection * view * Vec4(FAO[i].vertices[2], 1.0f))
                 .perspectiveDivide();
    Vec3 d = (projection * view * Vec4(FAO[i].vertices[3], 1.0f))
                 .perspectiveDivide();
    Texture& texture = Texture::byId[FAO[i].textureId];
    for (int x = 0; x < 16; x++) {
      for (int y = 0; y < 16; y++) {
        float u1 = x / 16.0f;
        float v1 = y / 16.0f;
        float u2 = (x + 1) / 16.0f;
        float v2 = (y + 1) / 16.0f;
        Vec3 p1 = a + (b - a) * u1 + (d - a) * v1 + (a - b + c - d) * u1 * v1;
        Vec3 p2 = a + (b - a) * u2 + (d - a) * v1 + (a - b + c - d) * u2 * v1;
        Vec3 p3 = a + (b - a) * u2 + (d - a) * v2 + (a - b + c - d) * u2 * v2;
        Vec3 p4 = a + (b - a) * u1 + (d - a) * v2 + (a - b + c - d) * u1 * v2;
        POINT points[4];
        points[0] = {(long)(width * (p1[0] + 1.0f) / 2),
                     (long)(height * (p1[1] + 1.0f) / 2)};
        points[1] = {(long)(width * (p2[0] + 1.0f) / 2),
                     (long)(height * (p2[1] + 1.0f) / 2)};
        points[2] = {(long)(width * (p3[0] + 1.0f) / 2),
                     (long)(height * (p3[1] + 1.0f) / 2)};
        points[3] = {(long)(width * (p4[0] + 1.0f) / 2),
                     (long)(height * (p4[1] + 1.0f) / 2)};
        points[0].y = height - points[0].y;
        points[1].y = height - points[1].y;
        points[2].y = height - points[2].y;
        points[3].y = height - points[3].y;
        setfillcolor(BGR(texture(x, y) & 0xffffff));
        solidpolygon(points, 4);
      }
    }
  }
}