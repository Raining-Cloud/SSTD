#pragma once

#include "General/Meta.h"
#include "General/Allocator.h"
#include "General/Memory.h"

namespace SSTD
{
  template<typename T, size_t StaticSize>
  class Array
  {
  public:
    using Iterator = T*;
    using ConstIterator = const T*;

    T& At(size_t i) { return m_Buffer[i]; }
    const T& At(size_t i) const { return m_Buffer[i]; }

    T& operator[](size_t i) { return m_Buffer[i]; }
    const T& operator[](size_t i) const { return m_Buffer[i]; }

    Iterator begin() { return { m_Buffer }; }
    Iterator end() { return { m_Buffer + StaticSize }; }

    ConstIterator begin() const { return { m_Buffer }; }
    ConstIterator end() const { return { m_Buffer + StaticSize }; }

    T* Data() const { return m_Buffer; }
    constexpr size_t Size() const { return StaticSize; }
  private:
    T m_Buffer[StaticSize]{};
  };
}