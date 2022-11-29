#pragma once

#include "Math.h"
#include "General/Numeric.h"

namespace SSTD
{
  template<typename T>
  struct Vec2
  {
    union
    {
      struct
      {
        T x, y;
      };

    };

    Vec2() :x(0), y(0) {}
    Vec2(T pX, T pY) :x(pX), y(pY) {}
    Vec2(const Vec2& v) : x(v.x), y(v.y) {}

    bool operator==(const Vec2& v) const { return (y == v.y && x == v.x); }
    bool operator!=(const Vec2& v) const { return !(y == v.y && x == v.x); }

    Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
    Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
    Vec2 operator*(const Vec2& v) const { return Vec2(x * v.x, y * v.y); }
    Vec2 operator/(const Vec2& v) const { return Vec2(x / v.x, y / v.y); }

    Vec2 operator*(const float& s) const { return Vec2(x * s, y * s); }
    Vec2 operator/(const float& s) const { return Vec2(x / s, y / s); }

    Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
    Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
    Vec2& operator*=(const Vec2& v) { x *= v.x; y *= v.y; return *this; }
    Vec2& operator/=(const Vec2& v) { x /= v.x; y /= v.y; return *this; }

    Vec2& operator*=(const float& s) { x *= s; y *= s; return *this; }
    Vec2& operator/=(const float& s) { x /= s; y /= s; return *this; }

    T& operator[](const uint32 i) { return ((T*)&x)[i]; }

    Vec2& normalize() { (*this) /= length(); return *this; }
    Vec2 normalized() { return Vec2(*this) /= length(); }

    void set(const T pX, const T pY) { x = pX; y = pY; }
    auto dot(const Vec2& v) { return (x * v.x + y * v.y); }
    auto length() { return SSTD::Math::Sqrt(x * x + y * y); }
    auto sqrLength() { return (x * x + y * y); }
  };

  template<typename T>
  struct Vec3
  {
    union {
      struct
      {
        T x, y, z;
      };
    };

    Vec3() {}
    Vec3(T pX, T pY, T pZ) :x(pX), y(pY), z(pZ) {}
    Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}

    bool operator==(const Vec3& v) const { return (y == v.y && x == v.x && z == v.z); }
    bool operator!=(const Vec3& v) const { return !(y == v.y && x == v.x && z == v.z); }

    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
    Vec3 operator/(const Vec3& v) const { return Vec3(x / v.x, y / v.y, z / v.z); }

    Vec3 operator*(const float& s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator/(const float& s) const { return Vec3(x / s, y / s, z / s); }

    Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vec3& operator*=(const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    Vec3& operator/=(const Vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

    Vec3& operator*=(const float& s) { x *= s; y *= s; z *= s; return *this; }
    Vec3& operator/=(const float& s) { x /= s; y /= s; z /= s; return *this; }

    T& operator[](const uint32 i) { return ((T*)&x)[i]; }

    Vec3& normalize() { (*this) /= length(); return *this; }
    Vec3 normalized() { return Vec3(*this) /= length(); }

    void set(const T pX, const T pY, const T pZ) { x = pX; y = pY; z = pZ; }
    auto dot(const Vec3& v) { return (x * v.x + y * v.y + z * v.z); }
    auto length() { return SSTD::Math::Sqrt(x * x + y * y + z * z); }
    auto sqrLength() { return (x * x + y * y + z * z); }
    auto distanceTo(const Vec3& v) { SSTD::Math::Sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z)); }
  };

  template<typename T>
  struct Vec4
  {
    struct
    {
      T x, y, z, w;
    };

    Vec4() {}
    Vec4(T pX, T pY, T pZ, T pW) :x(pX), y(pY), z(pZ), w(pW) {}
    Vec4(const Vec4<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    bool operator==(const Vec4<T>& v) const { return (y == v.y && x == v.x && z == v.z && w == v.w); }
    bool operator!=(const Vec4<T>& v) const { return !(y == v.y && x == v.x && z == v.z && w == v.w); }

    Vec4<T> operator+(const Vec4<T>& v) const { return Vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w); }
    Vec4<T> operator-(const Vec4<T>& v) const { return Vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w); }
    Vec4<T> operator*(const Vec4<T>& v) const { return Vec4<T>(x * v.x, y * v.y, z * v.z, w * v.w); }
    Vec4<T> operator/(const Vec4<T>& v) const { return Vec4<T>(x / v.x, y / v.y, z / v.z, w / v.w); }

    Vec4<T> operator*(const float& s) const { return Vec4<T>(x * s, y * s, z * s, w * s); }
    Vec4<T> operator/(const float& s) const { return Vec4<T>(x / s, y / s, z / s, w / s); }

    Vec4<T>& operator+=(const Vec4<T>& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    Vec4<T>& operator-=(const Vec4<T>& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
    Vec4<T>& operator*=(const Vec4<T>& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
    Vec4<T>& operator/=(const Vec4<T>& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

    Vec4<T>& operator*=(const float& s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    Vec4<T>& operator/=(const float& s) { x /= s; y /= s; z /= s; w /= s; return *this; }

    T& operator[](const uint32 i) { return ((T*)&x)[i]; }

    Vec4<T>& normalize() { (*this) /= length(); return *this; }
    Vec4<T> normalized() { return Vec4<T>(*this) /= length(); }

    void set(const T pX, const T pY, const T pZ, const T pW) { x = pX; y = pY; z = pZ; w = pW; }
    auto dot(const Vec4<T>& v) { return (x * v.x + y * v.y + z * v.z + w * v.w); }
    auto length() { return SSTD::Math::Sqrt(x * x + y * y + z * z + w * w); }
    auto sqrLength() { return (x * x + y * y + z * z + w * w); }
  };
}