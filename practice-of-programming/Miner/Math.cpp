#include "Math.h"

#include <math.h>

Vec3::Vec3(float x, float y, float z) {
  data[0] = x;
  data[1] = y;
  data[2] = z;
}

float& Vec3::operator[](int index) { return data[index]; }

Vec3 Vec3::operator+(const Vec3& other) const {
  return Vec3(data[0] + other.data[0], data[1] + other.data[1],
              data[2] + other.data[2]);
}

Vec3 Vec3::operator-(const Vec3& other) const {
  return Vec3(data[0] - other.data[0], data[1] - other.data[1],
              data[2] - other.data[2]);
}

Vec3 Vec3::operator*(float scalar) const {
  return Vec3(data[0] * scalar, data[1] * scalar, data[2] * scalar);
}

Vec3 Vec3::operator/(float scalar) const {
  return Vec3(data[0] / scalar, data[1] / scalar, data[2] / scalar);
}

Vec3 Vec3::normalize() const {
  float length =
      sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
  return Vec3(data[0] / length, data[1] / length, data[2] / length);
}

Vec3 Vec3::cross(const Vec3& other) const {
  return Vec3(data[1] * other.data[2] - data[2] * other.data[1],
              data[2] * other.data[0] - data[0] * other.data[2],
              data[0] * other.data[1] - data[1] * other.data[0]);
}

Vec4::Vec4(float x, float y, float z, float w) {
  data[0] = x;
  data[1] = y;
  data[2] = z;
  data[3] = w;
}

Vec4::Vec4(const Vec3& other, float w) {
  data[0] = other.data[0];
  data[1] = other.data[1];
  data[2] = other.data[2];
  data[3] = w;
}

float& Vec4::operator[](int index) { return data[index]; }

Vec4 Vec4::operator+(const Vec4& other) const {
  return Vec4(data[0] + other.data[0], data[1] + other.data[1],
              data[2] + other.data[2], data[3] + other.data[3]);
}

Vec4 Vec4::operator-(const Vec4& other) const {
  return Vec4(data[0] - other.data[0], data[1] - other.data[1],
              data[2] - other.data[2], data[3] - other.data[3]);
}

Vec3 Vec4::perspectiveDivide() const {
  return Vec3(data[0] / data[3], data[1] / data[3], data[2] / data[3]);
}

Mat4 Mat4::operator+(const Mat4& other) const {
  Mat4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.data[i][j] = data[i][j] + other.data[i][j];
    }
  }
  return result;
}

Mat4 Mat4::operator-(const Mat4& other) const {
  Mat4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.data[i][j] = data[i][j] - other.data[i][j];
    }
  }
  return result;
}

Mat4 Mat4::operator*(const Mat4& other) const {
  Mat4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.data[i][j] = 0;
      for (int k = 0; k < 4; ++k) {
        result.data[i][j] += data[i][k] * other.data[k][j];
      }
    }
  }
  return result;
}

Vec4 Mat4::operator*(const Vec4& other) const {
  Vec4 result;
  for (int i = 0; i < 4; ++i) {
    result.data[i] = 0;
    for (int j = 0; j < 4; ++j) {
      result.data[i] += data[i][j] * other.data[j];
    }
  }
  return result;
}

Mat4 Mat4::perspective(float fov, float aspectRatio, float nearPlane,
                       float farPlane) {
  Mat4 result;
  float q = 1.0f / tan(0.5f * fov);
  float a = q / aspectRatio;
  float b = (nearPlane + farPlane) / (nearPlane - farPlane);
  float c = (2.0f * nearPlane * farPlane) / (nearPlane - farPlane);
  result.data[0][0] = a;
  result.data[1][1] = q;
  result.data[2][2] = b;
  result.data[2][3] = -1.0f;
  result.data[3][2] = c;
  result.data[3][3] = 0.0f;
  return result;
}

Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
  Vec3 f = (target - eye).normalize();
  Vec3 r = up.cross(f).normalize();
  Vec3 u = f.cross(r).normalize();

  Mat4 result;
  result.data[0][0] = r[0];
  result.data[0][1] = r[1];
  result.data[0][2] = r[2];
  result.data[0][3] =
      -r[0] * eye.data[0] - r[1] * eye.data[1] - r[2] * eye.data[2];

  result.data[1][0] = u[0];
  result.data[1][1] = u[1];
  result.data[1][2] = u[2];
  result.data[1][3] =
      -u[0] * eye.data[0] - u[1] * eye.data[1] - u[2] * eye.data[2];

  result.data[2][0] = f[0];
  result.data[2][1] = f[1];
  result.data[2][2] = f[2];
  result.data[2][3] =
      -f[0] * eye.data[0] - f[1] * eye.data[1] - f[2] * eye.data[2];

  result.data[3][0] = 0.0f;
  result.data[3][1] = 0.0f;
  result.data[3][2] = 0.0f;
  result.data[3][3] = 1.0f;

  return result;
}