#pragma once

#include "General/Concepts.h"
#include "General/Allocator.h"
#include "General/Memory.h"
namespace SSTD
{
  template<typename T, size_t StaticSize, IntegralType SizeType = size_t>
  class StackArray
  {
  public:
    explicit StackArray() : m_Array{ 0 } {}
    StackArray(const StackArray& other) : m_Array(other.m_Array) {}

    StackArray& operator=(const StackArray& other) { return *this; }

    T& At(SizeType i) { return m_Array[i]; }
    const T& At(SizeType i) const { return m_Array[i]; }

    T& operator[](SizeType i) { return m_Array[i]; }
    const T& operator[](SizeType i) const { return m_Array[i]; }

    constexpr SizeType Size() const { return StaticSize; }
  private:
    T m_Array[StaticSize];
  };

  template<typename T, size_t StaticSize, IntegralType SizeType = size_t, template <typename> typename A = Allocator>
  class HeapArray
  {
  public:
    explicit HeapArray()
      : m_Allocator{}, m_Buffer(m_Allocator.Allocate(sizeof(T)* StaticSize))
    {}
    HeapArray(const HeapArray& other)
      : HeapArray()
    {
      m_Allocator = other.m_Allocator;
      TMemCpy<T>(m_Buffer, other.m_Buffer, StaticSize);
    }

    HeapArray(HeapArray&& other)
      : m_Allocator{ Move(other.m_Allocator) }, m_Buffer(other.m_Buffer)
    {
      other.m_Buffer = nullptr;
    }

    ~HeapArray()
    {
      m_Allocator.Deallocate(m_Buffer);
    }

    HeapArray& operator=(const HeapArray& other)
    {
      m_Allocator = other.m_Allocator;
      TMemCpy<T>(m_Buffer, other.m_Buffer, StaticSize);
      return *this;
    }
    HeapArray& operator=(HeapArray&& other)
    {
      m_Allocator = Move(other.m_Allocator);
      m_Buffer = other.m_Buffer;
      other.m_Buffer = nullptr;
      return *this;
    }

    T& At(SizeType i) { return reinterpret_cast<T*>(m_Buffer)[i]; }
    const T& At(SizeType i) const { return reinterpret_cast<T*>(m_Buffer)[i]; }

    T& operator[](SizeType i) { return reinterpret_cast<T*>(m_Buffer)[i]; }
    const T& operator[](SizeType i) const { return reinterpret_cast<T*>(m_Buffer)[i]; }

    constexpr SizeType Size() const { return StaticSize; }
  private:
    A<unsigned char> m_Allocator;
    unsigned char* m_Buffer;
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