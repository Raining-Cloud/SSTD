#pragma once

#include "General/Memory.h"
#include "General/Numeric.h"
#include "General/Iterator.h"
#include "General/Concepts.h"
#include "General/Allocator.h"

namespace SSTD
{
  template<typename T, IntegralType SizeType = size_t, template<typename> typename A = Allocator>
    requires IsNumeric<SizeType>::valid
  class Vector
  {
  public:
    using VectorIterator = Iterator<T>;
    using ConstVectorIterator = ConstIterator<T>;
    using AllocType = A<T>;

    Vector() noexcept {}

    explicit Vector(SizeType size)
      :m_Size(0), m_Capacity(size), m_Allocator(), m_Buffer(m_Allocator.Allocate(size))
    {}

    Vector(const T& value, SizeType size)
      :m_Size(size), m_Capacity(size), m_Allocator(), m_Buffer(m_Allocator.Allocate(size))
    {
      for (SizeType i = 0; i < size; ++i)
        m_Allocator.Construct(m_Buffer + i, T{ value });
    }

    Vector(const T* value, SizeType size)
      : m_Size(size), m_Capacity(size), m_Allocator(), m_Buffer(m_Allocator.Allocate(size))
    {
      for (SizeType i = 0; i < m_Size; ++i)
        m_Allocator.Construct(m_Buffer + i, T{ value[i] });
    }

    Vector(const Vector& other)
      :m_Size(other.m_Size), m_Capacity(other.m_Capacity), m_Allocator(other.m_Allocator), m_Buffer(m_Allocator.Allocate(other.m_Size))
    {
      for (SizeType i = 0; i < other.m_Size; ++i)
        m_Allocator.Construct(m_Buffer + i, T{ other.m_Buffer[i] });
    }

    Vector(Vector&& other) noexcept
      : m_Size(other.m_Size), m_Capacity(other.m_Capacity), m_Allocator(other.m_Allocator), m_Buffer(other.m_Buffer)
    {
      other.m_Size = other.m_Capacity = 0;
      other.m_Buffer = nullptr;
    }

    ~Vector()
    {
      if (m_Buffer)
        Clear();
    }

    Vector& operator=(const Vector& other)
    {
      if (m_Buffer)
        if (m_Capacity != other.m_Capacity)
          Clear();
        else
          Erase();

      if (other.m_Capacity > 0 && m_Buffer == nullptr)
        m_Buffer = m_Allocator.Allocate(other.m_Size);

      m_Capacity = other.m_Capacity;
      m_Size = other.m_Size;

      TMemCpy<T>(m_Buffer, other.m_Buffer, other.m_Size);
      return *this;
    }

    Vector& operator=(Vector&& other) noexcept
    {
      if (m_Buffer)
        Clear();

      Swap(m_Size, other.m_Size);
      Swap(m_Capacity, other.m_Capacity);
      Swap(m_Buffer, other.m_Buffer);
      return *this;
    }

    T& operator[](const SizeType index) { return m_Buffer[index]; }
    const T& operator[](const SizeType index) const { return m_Buffer[index]; }

    void Resize(const SizeType size)
    {
      Reserve(size);//TODO: this is very not so gud
      m_Size = size;
    }

    void Reserve(const SizeType size)
    {
      if (size <= m_Capacity)
        return;

      T* tmp = m_Allocator.Allocate(size);

      if (m_Buffer)
        TMemCpy<T>(tmp, m_Buffer, m_Size);

      m_Allocator.Deallocate(m_Buffer);
      m_Buffer = tmp;

      m_Capacity = size;
    }

    void Clear()
    {
      Erase();
      m_Size = 0;
      m_Capacity = 0;
      m_Allocator.Deallocate(m_Buffer);
      m_Buffer = nullptr;
    }

    void Erase()
    {
      for (SizeType i = 0; i < m_Size; ++i)
        m_Buffer[i].~T();
      m_Size = 0;
    }

    void Minimize()
    {
      Resize(m_Size);
    }

    void PushBack(const T& value)
    {
      EmplaceBack(value);
    }

    void PushBack(T&& value)
    {
      EmplaceBack(Forward<T>(value));
    }

    void PushBackUnique(const T& value)
    {
      if (!Contains(value))
        PushBack(value);
    }

    void PushBackUnique(T&& value)
    {
      if (!Contains(value))
        PushBack(value);
    }

    template<typename... Args>
    T& EmplaceBack(Args&&... args)
    {
      TryReserve();
      m_Allocator.Construct(m_Buffer + (m_Size++), T{ Forward<Args>(args)... });
      return m_Buffer[m_Size];
    }

    void PopBack()
    {
      m_Buffer[m_Size--].~T();
    }

    void Append(const T* data, SizeType size)
    {
      Reserve(m_Size + size);
      TMemCpy<T>(m_Buffer[m_Size], data, size);
      m_Size += size;
    }

    void Append(const Vector& other)
    {
      Append(other.m_Buffer, other.m_Size);
    }

    T RemoveAt(const SizeType index)
    {
      T tmp = Move(m_Buffer[index]);
      m_Buffer[index].~T();
      memmove(m_Buffer[index], m_Buffer[index + 1], (m_Size--) - index); //shift the elements
      return tmp;
    }

    SizeType IndexOf(const T& value)
    {
      for (SizeType i = 0; i < m_Size; i++)
        if (value == m_Buffer[i])
          return i;
      return -1;
    }

    T& At(const SizeType index)
    {
      if (index > m_Size)
        throw;

      return m_Buffer[index];
    }

    const T& At(const SizeType index) const
    {
      if (index > m_Size)
        throw;

      return m_Buffer[index];
    }

    bool Contains(const T& value)
    {
      for (SizeType i = 0; i < m_Size; i++)
        if (m_Buffer[i] == value)
          return true;
      return false;
    }

    bool CheckIndex(const SizeType index) { return index < m_Size; }

    T* Data() { return m_Buffer; }

    const T* Data() const { return m_Buffer; }

    SizeType Size() const { return m_Size; }

    SizeType Capacity() const { return m_Capacity; }

    bool IsEmpty() const { return m_Size == 0; }

    template<typename F>
    void Apply(const F& function)
    {
      for (SizeType i = 0; i < m_Size; i++)
        function(m_Buffer[i]);
    }

    T& Front() { return m_Buffer[0]; }

    const T& Front() const { return m_Buffer[0]; }

    T& Back() { return m_Buffer[m_Size - 1]; }

    const T& Back() const { return m_Buffer[m_Size - 1]; }

    VectorIterator begin() { return VectorIterator(m_Buffer); }

    VectorIterator end() { return VectorIterator(m_Buffer + m_Size); }

    ConstVectorIterator begin() const { return ConstVectorIterator(m_Buffer); }

    ConstVectorIterator end() const { return ConstVectorIterator(m_Buffer + m_Size); }
  private:

    void TryReserve()
    {
      if (m_Size == m_Capacity)
        Reserve((m_Capacity + 1) * 2);
    }

  private:
    AllocType m_Allocator{};
    SizeType m_Size{ 0 };
    SizeType m_Capacity{ 0 };
    T* m_Buffer{ nullptr };
  };
}