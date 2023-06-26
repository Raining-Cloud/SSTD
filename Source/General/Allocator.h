#pragma once

#include "Utility.h"
#include "Meta.h"
#include "Memory.h"

namespace SSTD
{
  template<typename T>
  class Allocator
  {
  public:

    explicit Allocator() throw() {}
    explicit Allocator(const Allocator& other) {}

    template<typename U>
    explicit Allocator(const Allocator<U>& other) {}

    MemoryBlock<T> Allocate(size_t size)
    {
      return { static_cast<Pointer<T>>(::operator new(size * sizeof(T))), size };
    }

    void Deallocate(MemoryBlock<T> memblock)
    {
      ::operator delete((void*)memblock.ptr, memblock.size);
    }

    template<typename ... Args>
    Pointer<T> Create(Args&&... args)
    {
      return new (::operator new(1 * sizeof(T))) T{ Forward<Args>(args)... };
    }

    void Delete(Pointer<T> ptr)
    {
      ::operator delete((void*)ptr, 1);
    }

    void Construct(Pointer<T>* ptr, const T& value)
    {
      new (static_cast<void*>(ptr)) T{ value };
    }

    template<typename U, typename ... Args>
    void Construct(U* ptr, Args&&... args)
    {
      new (static_cast<void*>(ptr)) T{ Forward<Args>(args)... };
    }
  };

  template<typename T, size_t InlineSize, template<typename> typename A = Allocator>
  class InlineAllocator
  {
  public:
    typedef A<T> AllocType;

    InlineAllocator() noexcept {}
    InlineAllocator(const InlineAllocator& other) {}

    Pointer<T> Allocate(size_t size)
    {
      if (size > InlineSize)
        return m_Allocator.Allocate(size);
      return m_InlineBuffer;
    }

    void Deallocate(Pointer<T> ptr)
    {
      if (true)
        m_Allocator.Dealocate(ptr);
    }

    void Construct(Pointer<T>* ptr, const T& value)
    {
      new (static_cast<void*>(ptr)) T{ value };
    }

    template<typename U, typename ... Args>
    void Construct(U* ptr, Args&&... args)
    {
      new (static_cast<void*>(ptr)) T{ Forward<Args>(args)... };
    }

  private:
    [[no_unique_address]] AllocType m_Allocator{};
    char m_InlineBuffer[sizeof(T) * InlineSize] = { 0 };
  };
}