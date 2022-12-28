#pragma once

#include "Math.h"

#include "General/Numeric.h"

namespace SSTD
{
  template<typename T, size_t Dim> 
  struct Vec
  {
  public:
    Vec() {}
    Vec(const T& value) noexcept : m_Buffer{value} {}
    Vec(const Vec& other) noexcept { TMemCpy(m_Buffer, other.m_Buffer, Dim); }

    template<typename ... Args>
      requires (sizeof...(Args) == Dim)
    Vec(Args&&... args) : m_Buffer{static_cast<T>(args)...} {}
    
    template<size_t OtherDim>
      requires IsLessEqual<OtherDim, Dim>
    Vec(const Vec<T, OtherDim>& other) { TMemCpy(m_Buffer, other.m_Buffer, Dim); }

    Vec& operator=(const Vec& other) { TMemCpy(m_Buffer, other.m_Buffer, Dim); return *this; }

    Vec& operator==(const Vec& other) { return false; }
    Vec& operator!=(const Vec& other) { return false; }

    constexpr T& x() const requires(Dim > 0) { return m_Buffer[0]; }
    constexpr T& y() const requires(Dim > 1) { return m_Buffer[1]; }
    constexpr T& z() const requires(Dim > 2) { return m_Buffer[2]; }
    constexpr T& w() const requires(Dim > 3) { return m_Buffer[3]; }

    T& At(size_t index) const { if (index < Dim) return this->operator[](index); }
    T& At(size_t index) { if (index < Dim) return this->operator[](index); }
    T& operator[](size_t index) const { return m_Buffer[index]; }
    T& operator[](size_t index) { return m_Buffer[index]; }

    constexpr T* Data() const { return m_Buffer; }
    constexpr size_t Size() { return Dim; }
  private:
    T m_Buffer[Dim]{};
  };

  template<typename T>
  using Vec2 = Vec<T, 2>;

  template<typename T>
  using Vec3 = Vec<T, 3>;

  template<typename T>
  using Vec4 = Vec<T, 4>;
}