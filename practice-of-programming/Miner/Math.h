#pragma once

class Vec3 {
  float data[3];

 public:
  Vec3(float x = 0, float y = 0, float z = 0);
  float& operator[](int index);
  Vec3 operator+(const Vec3& other) const;
  Vec3 operator-(const Vec3& other) const;
  Vec3 operator*(float scalar) const;
  Vec3 operator/(float scalar) const;
  Vec3 normalize() const;
  Vec3 cross(const Vec3& other) const;
  friend class Mat4;
  friend class Vec4;
};

class Vec4 {
  float data[4];

 public:
  Vec4(float x = 0, float y = 0, float z = 0, float w = 0);
  Vec4(const Vec3& other, float w = 0);
  float& operator[](int index);
  Vec4 operator+(const Vec4& other) const;
  Vec4 operator-(const Vec4& other) const;
  Vec3 perspectiveDivide() const;
  friend class Mat4;
  friend class Vec3;
};

class Mat4 {
  float data[4][4] = {};

 public:
  float* operator[](int index) { return data[index]; }
  Mat4 operator+(const Mat4& other) const;
  Mat4 operator-(const Mat4& other) const;
  Mat4 operator*(const Mat4& other) const;
  Vec4 operator*(const Vec4& other) const;
  static Mat4 perspective(float fov, float aspectRatio, float nearPlane,
                          float farPlane);
  static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
  friend class Vec3;
  friend class Vec4;
};
