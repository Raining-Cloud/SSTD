#pragma once

#include "General/Numeric.h"

namespace SSTD
{
  struct Color
  {
    uint32 r;
    uint32 g;
    uint32 b;
    uint8 a;

    Color() : r(0), g(0), b(0), a(255) {}
    Color(uint32 pR, uint32 pG, uint32 pB, uint8 pA = 255) : r(pR), g(pG), b(pB), a(pA) {}
    Color(uint32 hex) { FromHEX(hex); }
    Color(const Color& other) : r(other.r), g(other.g), b(other.b), a(other.a) {}

    void FromHEX(uint32 hex)
    {
      r = (hex & 0xFF000000) >> 24;
      g = (hex & 0x00FF0000) >> 16;
      b = (hex & 0x0000FF00) >> 8;
      a = (hex & 0x000000FF) >> 4;
    }
    uint32 ToHEX()
    {
      return ((r & 0xFF) << 24) + ((g & 0xFF) << 16) + ((b & 0xFF) << 8) + (a & 0xFF);
    }
  };

  namespace Colors
  {
    static inline const Color White = Color(0xFFFFFFFF);
    static inline const Color Black = Color(0x000000FF);
    static inline const Color Red = Color(0xFF0000FF);
    static inline const Color Green = Color(0x00FF00FF);
    static inline const Color blue = Color(0x0000FFFF);
    static inline const Color DarkGrey = Color(0x202225FF);
  }


}