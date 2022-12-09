#pragma once

#include "General/Concepts.h"
#include "General/Allocator.h"
#include "General/Memory.h"
namespace SSTD
{
  template<typename T, size_t StaticSize, IntegralType SizeType = size_t>
  class Array
  {
  public:
    T& At(SizeType i) { return m_Array[i]; }
    const T& At(SizeType i) const { return m_Array[i]; }

    T& operator[](SizeType i) { return m_Array[i]; }
    const T& operator[](SizeType i) const { return m_Array[i]; }

    constexpr SizeType Size() const { return StaticSize; }
  private:
    T m_Array[StaticSize];
  };

  template<typename... T>
  class TypeArray
  {
  public:
    template<typename Lambda>
    void Apply(Lambda& lambda) { return; }
  };

  template<typename T, typename... R>
  class TypeArray<T, R...>
  {
  private:
  public:

    TypeArray() {}
    TypeArray(const T& t, const R& ... r)
      : m_First(t), m_Rest(r...) {}

    ~TypeArray() {}

    template<typename Type>
      requires IsSameType<T, Type>
    T& Get() { return m_First; }

    template<typename Type>
      requires IsSameType<T, Type>
    T& Get() const { return m_First; }

    template<typename Type>
      requires IsDifferentType<T, Type>
    auto Get() { return m_Rest.get<Type>(); }

    template<typename Type>
      requires IsDifferentType<T, Type>
    auto Get() const { return m_Rest.get<Type>(); }

    template<typename Type>
      requires IsSameType<T, Type>
    void Set(const Type& value) { m_First = SSTD::Move(value); }

    template<typename Type>
      requires IsDifferentType<T, Type>
    void Set(const Type& value) { m_Rest.set<Type>(value); }

    template<typename Lambda>
    void Apply(Lambda& lambda) { lambda(m_First); m_Rest.Apply<Lambda>(lambda); }
  private:
    T m_First;
    TypeArray<R...> m_Rest;
  };
}