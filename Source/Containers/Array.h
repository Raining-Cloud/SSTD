#pragma once

#include "General/Concepts.h"
#include "General/Allocator.h"
#include "General/Memory.h"
namespace SSTD
{
  template<typename T, size_t StaticSize>
  class Array
  {
  public:
    T& At(size_t i) { return m_Array[i]; }
    const T& At(size_t i) const { return m_Array[i]; }

    T& operator[](size_t i) { return m_Array[i]; }
    const T& operator[](size_t i) const { return m_Array[i]; }

    T* Data() const { return m_Array; }
    constexpr size_t Size() const { return StaticSize; }
  private:
    T m_Array[StaticSize]{};
  };

  template<typename... T>
  class TypeArray
  {
  public:
    template<typename Lambda>
    void Apply(Lambda& lambda) { return; }
  };
}