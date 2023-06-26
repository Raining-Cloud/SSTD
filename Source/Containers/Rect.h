#pragma once

#include "General/Numeric.h"
#include "Math/Math.h"

namespace SSTD
{
  template<typename T>
    requires IsNumeric<T>::valid
  struct Rect
  {
    Rect() :left{}, right{}, top{}, bottom{} {}

    Rect(T left, T top, T right, T bottom) :left(left), right(right), top(top), bottom(bottom) {}

    T Width() const { return (right - left); }
    T Height() const { return (bottom - top); }

    bool operator==(const Rect& other)
    {
      return (left == other.left && right == other.right && top == other.top && bottom == other.bottom);
    }

    bool operator!=(const Rect& other)
    {
      return !(*this == other);
    }

    T left;
    T right;
    T top;
    T bottom;
  };
}
