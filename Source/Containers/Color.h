#pragma once

#include "General/Numeric.h"
#include "General/Meta.h"

namespace SSTD
{
  template<IntegralType T>
  struct Color
  {
    Color() = default;
    Color(T r, T g, T b, T a = 1) : r(r), g(g), b(b), a(a) {}

    bool operator==(const Color& other) { return r == other.r && g == other.g && b == other.b && a == other.a; };
    bool operator!=(const Color& other) { return !(*this == other); };

    T& operator[](uint8 index)
    {
      switch (index)
      {
      case 0:
        return r;
      case 1:
        return g;
      case 2:
        return b;
      case 3:
        return a;
      default:
        return T{}; //result here?
      }
    }

    struct
    {
      T r{};
      T g{};
      T b{};
      T a{};
    };
  };

  using Color8 = Color<uint8>;
  using Color16 = Color<uint16>;
  using Color32 = Color<uint32>;
}