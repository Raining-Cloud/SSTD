#pragma once

#include "Utility.h"
#include "Meta.h"

namespace SSTD
{
  template<typename T>
  class Allocator
  {
  public:
    using SizeType = size_t;
    typedef T* Pointer;

    explicit Allocator() throw() {}
    explicit Allocator(const Allocator& other) {}
    
    template<typename N>
    explicit Allocator(const Allocator<N>& other) {}

    Pointer Allocate(size_t size)
    {
      return static_cast<Pointer>(::operator new(size * sizeof(T)));
    }

    void Deallocate(Pointer ptr)
    {
      ::operator delete((void*)ptr);
    }

    void Construct(Pointer* ptr, const T& value)
    {
      new (static_cast<void*>(ptr)) T { value };
    }

    template<typename U, typename ... Args>
    void Construct(U* ptr, Args&&... args)
    {
      new (static_cast<void*>(ptr)) T{ Forward<Args>(args)... };
    }
  };
}