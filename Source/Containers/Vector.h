#pragma once

#include "General/Memory.h"
#include "General/Numeric.h"
#include "General/Meta.h"
#include "General/Allocator.h"

#include "General/Exception.h"
#include "General/Result.h"

namespace SSTD
{
  template<typename T, IntegralType SizeType = size_t, template<typename> typename A = Allocator, typename G = GenericGrow>
  class Vector
  {
  public:
    using VectorIterator = T*;
    using ConstVectorIterator = const T*;
    using AllocType = A<T>;

    Vector() noexcept
    {}

    explicit Vector(SizeType size)
      :m_Size(0), m_Allocator(), m_MemBlock(m_Allocator.Allocate(size))
    {}

    Vector(const T& value, SizeType size)
      :m_Size(size), m_Allocator(), m_MemBlock(m_Allocator.Allocate(size))
    {
      for (SizeType i = 0; i < size; ++i)
        m_Allocator.Construct(m_MemBlock.ptr + i, T{ value });
    }

    Vector(const T* value, SizeType size)
      : m_Size(size), m_Allocator(), m_MemBlock(m_Allocator.Allocate(size))
    {
      for (SizeType i = 0; i < m_Size; ++i)
        m_Allocator.Construct(m_MemBlock.ptr + i, T{ value[i] });
    }

    Vector(const Vector& other)
      :m_Size(other.m_Size), m_Allocator(other.m_Allocator), m_MemBlock(m_Allocator.Allocate(other.m_Size))
    {
      for (SizeType i = 0; i < other.m_Size; ++i)
        m_Allocator.Construct(m_MemBlock.ptr + i, T{ other.m_MemBlock.ptr[i] });
    }

    Vector(Vector&& other) noexcept
      : m_Size(other.m_Size), m_Allocator(other.m_Allocator), m_MemBlock(other.m_MemBlock)
    {
      other.m_Size = 0;
      other.m_MemBlock = {};
    }

    ~Vector()
    {
      if (m_MemBlock)
        Clear();
    }

    Vector& operator=(const Vector& other)
    {
      if (m_MemBlock)
          Clear(); //TODO: Reverse grow for checking if we actually need to clean ?

      if (other.m_Size > 0)
        m_MemBlock = m_Allocator.Allocate(other.m_Size);

      m_Size = other.m_Size;

      TMemCpy<T>(m_MemBlock.ptr, m_MemBlock.ptr, other.m_Size);
      return *this;
    }

    Vector& operator=(Vector&& other) noexcept
    {
      if (m_MemBlock)
        Clear();

      m_Size = other.m_Size;
      m_MemBlock = other.m_MemBlock;

      other.m_Size = 0;
      other.m_MemBlock = {};

      return *this;
    }

    T& operator[](const SizeType index) { return m_MemBlock.ptr[index]; }
    const T& operator[](const SizeType index) const { return m_MemBlock.ptr[index]; }

    void Resize(const SizeType size)
    {
      Reserve(size);//TODO: this is very not so gud
      m_Size = size;
    }

    void Reserve(const SizeType capacity)
    {
      if (capacity <= m_MemBlock.size)
        return;

      const auto tmp = m_Allocator.Allocate(G::Grow(capacity));

      if (m_MemBlock)
        TMemCpy<T>(tmp.ptr, m_MemBlock.ptr, m_Size);

      m_Allocator.Deallocate(m_MemBlock);
      m_MemBlock = tmp;
    }

    void Clear()
    {
      Erase();
      m_Allocator.Deallocate(m_MemBlock);
      m_MemBlock = {};
    }

    void Erase()
    {
      for (SizeType i = 0; i < m_Size; ++i)
        m_MemBlock.ptr[i].~T();
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
      m_Allocator.Construct(m_MemBlock.ptr + (m_Size++), T{ Forward<Args>(args)... });
      return m_MemBlock.ptr[m_Size];
    }

    void PopBack()
    {
      m_MemBlock.ptr[m_Size--].~T();
    }

    void Append(const T* data, SizeType size)
    {
      Reserve(m_Size + size);
      TMemCpy<T>(m_MemBlock.ptr[m_Size], data, size);
      m_Size += size;
    }

    void Append(const Vector& other)
    {
      Append(other.m_MemBlock.ptr, other.m_Size);
    }

    T RemoveAt(const SizeType index)
    {
      T tmp = Move(m_MemBlock.ptr[index]);
      m_MemBlock.ptr[index].~T();
      TMemMove<T>(m_MemBlock.ptr[index], m_MemBlock.ptr[index + 1], (m_Size--) - index); //shift the elements
      return tmp;
    }

    Result<SizeType> IndexOf(const T& value)
    {
      for (SizeType i = 0; i < m_Size; i++)
        if (value == m_MemBlock.ptr[i])
          return Result<SizeType>(1);

      return Result<SizeType>();
    }

    Result<T&> At(const SizeType index)
    {
      if (index > m_Size)
        return Result<T&>();

      return Result<T&>(m_MemBlock.ptr[index]);
    }

    Result<const T&> At(const SizeType index) const
    {
      if (index > m_Size)
        return Result<T&>();

      return Result<T&>(m_MemBlock.ptr[index]);
    }

    bool Contains(const T& value)
    {
      for (SizeType i = 0; i < m_Size; i++)
        if (m_MemBlock.ptr[i] == value)
          return true;
      return false;
    }

    bool CheckIndex(const SizeType index) { return index < m_Size; }

    T* Data() { return m_MemBlock.ptr; }

    const T* Data() const { return m_MemBlock.ptr; }

    SizeType Size() const { return m_Size; }

    SizeType Capacity() const { return m_MemBlock.size; }

    bool IsEmpty() const { return m_Size == 0; }

    template<typename F>
    void Apply(const F& function)
    {
      for (SizeType i = 0; i < m_Size; i++)
        function(m_MemBlock.ptr[i]);
    }

    T& Front() { return m_MemBlock.ptr[0]; }

    const T& Front() const { return m_MemBlock.ptr[0]; }

    T& Back() { return m_MemBlock.ptr[m_Size - 1]; }

    const T& Back() const { return m_MemBlock.ptr[m_Size - 1]; }

    VectorIterator begin() { return VectorIterator(m_MemBlock.ptr); }

    VectorIterator end() { return VectorIterator(m_MemBlock.ptr + m_Size); }

    ConstVectorIterator begin() const { return ConstVectorIterator(m_MemBlock.ptr); }

    ConstVectorIterator end() const { return ConstVectorIterator(m_MemBlock.ptr + m_Size); }
  private:

    void TryReserve()
    {
      if (m_Size == m_MemBlock.size)
        Reserve((m_MemBlock.size + 1) * 2);
    }

    [[no_unique_address]] AllocType m_Allocator{};
    SizeType m_Size = 0;
    MemoryBlock<T, SizeType> m_MemBlock = {};
  };
}