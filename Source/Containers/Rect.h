#pragma once

#include "General/Numeric.h"

#define SSTD_RECT_OPERATION(op) 0

namespace SSTD
{
  template<typename T>
    requires IsNumeric<T>::valid
  struct Rect
  {
    Rect()
      :left{}, right{}, top{}, bottom{}
    {}

    Rect(T l, T r, T t, T b)
      :left(l), right(r), top(t), bottom(b)
    {}

    T Width() const { return (right - left); }
    T Height() const { return (bottom - top); }

    T left;
    T right;
    T top;
    T bottom;
  };
}
