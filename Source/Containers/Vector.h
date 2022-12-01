#pragma once

#include "General/Memory.h"
#include "General/Numeric.h"
#include "General/Iterator.h"
#include "General/Concepts.h"
#include "General/Allocator.h"

#include <new>

namespace SSTD
{
  template<typename T, IntegralType SizeType = size_t, template<typename> typename A = Allocator>
    requires IsNumeric<SizeType>::valid
  class Vector
  {
  public:
    using VectorIterator = Iterator<T>;
    using ConstVectorIterator = ConstIterator<T>;
    using AllocType = A<unsigned char>;

    Vector() noexcept
      :m_Buffer(nullptr), m_Size(0), m_Capacity(0), m_Allocator()
    {}

    explicit Vector(SizeType size)
      :m_Size(size), m_Capacity(size), m_Allocator(), m_Buffer(m_Allocator.Allocate(sizeof(T)* size))
    {
      for (SizeType i = 0; i < size; ++i)
        new(reinterpret_cast<T*>(m_Buffer) + i) T();
    }

    Vector(const T& value, SizeType size = 1)
      :m_Size(size), m_Capacity(size), m_Allocator(), m_Buffer(m_Allocator.Allocate(sizeof(T)* size))
    {
      for (SizeType i = 0; i < size; ++i)
        new(reinterpret_cast<T*>(m_Buffer) + i) T(value);
    }

    Vector(const T* value, SizeType size)
      : m_Size(size), m_Capacity(size), m_Allocator(), m_Buffer(m_Allocator.Allocate(sizeof(T)* size))
    {
      for (SizeType i = 0; i < m_Size; ++i)
        new(reinterpret_cast<T*>(m_Buffer) + i) T(value[i]);
    }

    Vector(const Vector& other)
      :m_Size(other.m_Size), m_Capacity(other.m_Capacity), m_Allocator(other.m_Allocator), m_Buffer(m_Allocator.Allocate(sizeof(T)* other.m_Size))
    {
      for (SizeType i = 0; i < other.m_Size; ++i)
        new(reinterpret_cast<T*>(m_Buffer) + i) T(other[i]);
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
          ClearBuffer();

      if (other.m_Capacity > 0 && m_Buffer == nullptr)
        m_Buffer = m_Allocator.Allocate(sizeof(T) * other.m_Size);

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

    inline T& operator[](const SizeType index) { return reinterpret_cast<T*>(m_Buffer)[index]; }
    inline const T& operator[](const SizeType index) const { return reinterpret_cast<T*>(m_Buffer)[index]; }

    void Resize(const SizeType size)
    {
      Reserve(size);//TODO: this is very not so gud
      m_Size = size;
    }

    void Reserve(const SizeType size)
    {
      if (size <= m_Capacity)
        return;

      unsigned char* tmp = m_Allocator.Allocate(sizeof(T) * size);

      if (m_Buffer)
        TMemCpy<T>(reinterpret_cast<T*>(tmp), reinterpret_cast<T*>(m_Buffer), m_Size);

      m_Allocator.Deallocate(m_Buffer);
      m_Buffer = tmp;

      m_Capacity = size;
    }

    void Clear()
    {
      ClearBuffer();
      m_Size = 0;
      m_Allocator.Deallocate(m_Buffer);
      m_Buffer = nullptr;
    }

    void Minimize()
    {
      Resize(m_Size);
    }
  
    void Push(const T& value)
    {
      Emplace(value);
    }

    void Push(T&& value)
    {
      Emplace(value);
    }

    void PushUnique(const T& value)
    {
      if (!Contains(value))
        Push(value);
    }

    void PushUnique(T&& value)
    {
      if (!Contains(value))
        Push(value);
    }

    template<typename... Args>
    T& Emplace(Args&&... args)
    {
      TryReserve();
      new (&m_Buffer[m_Size++]) T{ Forward<Args>(args)... };
      return reinterpret_cast<T*>(m_Buffer)[m_Size];
    }

    void Pop()
    {
      reinterpret_cast<T*>(m_Buffer)[m_Size--].~T();
    }

    void Append(const T* data, SizeType size)
    {
      Reserve(m_Size + size);
      TMemCpy<T>(reinterpret_cast<T*>(m_Buffer)[m_Size], data, size);
      m_Size += size;
    }

    void Append(const Vector& other)
    {
      Append(reinterpret_cast<T*>(other.m_Buffer), other.m_Size);
    }

    T RemoveAt(const SizeType index)
    {
      T tmp = Move(reinterpret_cast<T*>(m_Buffer)[index]);
      reinterpret_cast<T*>(m_Buffer)[index].~T();
      memmove(reinterpret_cast<T*>(m_Buffer)[index], reinterpret_cast<T*>(m_Buffer)[index + 1], m_Size-- - index * sizeof(T)); //shift the elements
      return tmp;
    }

    SizeType IndexOf(const T& value)
    {
      for (SizeType i = 0; i < m_Size; i++)
        if (value == reinterpret_cast<T*>(m_Buffer)[i])
          return i;
      return -1;
    }

    T& At(const SizeType index)
    {
      //STODO: Add check here!
      return reinterpret_cast<T*>(m_Buffer)[index];
    }

    const T& At(const SizeType index) const
    {
      return reinterpret_cast<T*>(m_Buffer)[index];
    }

    inline bool Contains(const T& value)
    {
      for (SizeType i = 0; i < m_Size; i++)
        if (reinterpret_cast<T*>(m_Buffer)[i] == value)
          return true;
      return false;
    }

    inline bool CheckIndex(const SizeType index) { return index < m_Size; }

    inline T* GetBuffer() { return reinterpret_cast<T*>(m_Buffer); }

    inline const T* GetBuffer() const { return reinterpret_cast<T*>(m_Buffer); }

    inline SizeType Size() const { return m_Size; }

    inline SizeType Capacity() const { return m_Capacity; }

    inline bool IsEmpty() const { return m_Size == 0; }

    template<typename F>
    void Func(const F& function)
    {
      for (SizeType i = 0; i < m_Size; i++)
        function(reinterpret_cast<T*>(m_Buffer)[i]);
    }

    inline T& Front() { return reinterpret_cast<T*>(m_Buffer)[0]; }

    inline const T& Front() const { return reinterpret_cast<T*>(m_Buffer)[0]; }

    inline T& Back() { return reinterpret_cast<T*>(m_Buffer)[m_Size - 1]; }

    inline T& Back() const { return reinterpret_cast<T*>(m_Buffer)[m_Size - 1]; }

    inline VectorIterator begin() { return VectorIterator(&reinterpret_cast<T*>(m_Buffer)[0]); }

    inline VectorIterator end() { return VectorIterator(&reinterpret_cast<T*>(m_Buffer)[m_Size]); }

    inline ConstVectorIterator begin() const { return ConstVectorIterator(&reinterpret_cast<T*>(m_Buffer)[0]); }

    inline ConstVectorIterator end() const { return ConstVectorIterator(&reinterpret_cast<T*>(m_Buffer)[m_Size]); }
  private:
    inline void ClearBuffer()
    {
      for (SizeType i = 0; i < m_Size; ++i)
        reinterpret_cast<T*>(m_Buffer)[i].~T();
    }

    inline void TryReserve()
    {
      if (m_Size == m_Capacity)
        Reserve((m_Capacity + 1) << 2);
    }

  private:
    AllocType m_Allocator{};
    SizeType m_Size{};
    SizeType m_Capacity{};
    unsigned char* m_Buffer{};
  };
}