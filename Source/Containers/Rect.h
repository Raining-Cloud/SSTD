#pragma once

#include "General/Numeric.h"
namespace SSTD
{
  template<typename T>
    requires IsNumeric<T>::valid
  struct Rect
  {
    Rect()
      :left(0), right(0), top(0), bottom(0)
    {}

    Rect(long l, long r, long t, long b)
      :left(l), right(r), top(t), bottom(b)
    {}

    Rect(const Rect& other)
      :left(other.left), right(other.right), top(other.top), bottom(other.bottom)
    {}

    Rect(Rect&& other)
      :left(other.left), right(other.right), top(other.top), bottom(other.bottom)
    {}

    ~Rect() {};

    long Width() const { return (right - left); }
    long Height() const { return (bottom - top); }

    T left;
    T right;
    T top;
    T bottom;
  };

  using Rect8 = Rect<int8>;
  using Rect16 = Rect<int16>;
  using Rect32 = Rect<int32>;
  using Rect64 = Rect<int64>;

  using uRect8 = Rect<uint8>;
  using uRect16 = Rect<uint16>;
  using uRect32 = Rect<uint32>;
  using uRect64 = Rect<uint64>;
}
