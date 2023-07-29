#pragma once

#include "General/Numeric.h"
#include "General/Meta.h"

#include <xmmintrin.h>
#include <emmintrin.h>
#include <math.h>

namespace SSTD
{
  namespace Math
  {
    namespace LUT
    {
      static constexpr int Log2_32[] = {
       0,  9,  1,  10, 13, 21,  2, 29,
       11, 14, 16, 18, 22, 25,  3, 30,
       8,  12, 20, 28, 15, 17, 24,  7,
       19, 27, 23,  6, 26,  5,  4, 31
      };

      static constexpr int Log2_64[] = {
       63,  0, 58,  1, 59, 47, 53,  2,
       60, 39, 48, 27, 54, 33, 42,  3,
       61, 51, 37, 40, 49, 18, 28, 20,
       55, 30, 34, 11, 43, 14, 22,  4,
       62, 57, 46, 52, 38, 26, 32, 41,
       50, 36, 17, 19, 29, 10, 13, 21,
       56, 45, 25, 31, 35, 16,  9, 12,
       44, 24, 15,  8, 23,  7,  6,  5 };
    }

    //Constants

    static constexpr float Pi = 3.14159265358979323846f;
    static constexpr float Epsilon = 1e-6f;
    static constexpr float Infinity = 1e+9f;

    template <IntegralType T>
    static constexpr auto Pow(T val, T power)
    {
      auto out = 1;
      while (power != 0)
      {
        if ((power & 1))
          out *= val;
        power >>= 1;
        val *= val;
      }
    }

    static double Sqrt(double value)
    {
      return sqrt(value);
    }

    static constexpr uint32 Log2(uint32 val)
    {
      val |= val >> 1;
      val |= val >> 2;
      val |= val >> 4;
      val |= val >> 8;
      val |= val >> 16;
      return LUT::Log2_32[(uint32)(val * 0x07C4ACDD) >> 27];
    }

    static constexpr uint64 Log2(uint64 val)
    {
      val |= val >> 1;
      val |= val >> 2;
      val |= val >> 4;
      val |= val >> 8;
      val |= val >> 16;
      val |= val >> 32;
      return LUT::Log2_64[((uint64)((val - (val >> 1)) * 0x07EDD5E59A4E28C2)) >> 58];
    }

    template<typename T>
    static constexpr T Max(T a, T b)
    {
      return (a > b) ? a : b;
    }

    template<typename T>
    static constexpr T Min(T a, T b)
    {
      return (a < b) ? a : b;
    }

    template<typename T>
    static constexpr T Clamp(T value, T min, T max)
    {
      return (value > max) ? max : (value < min) ? min : value;
    }
  };
}
