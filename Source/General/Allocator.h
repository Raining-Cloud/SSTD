#pragma once

#include "Utility.h"
#include "Concepts.h"

namespace SSTD
{
  template<typename T>
  class Allocator
  {
  public:
    using SizeType = size_t;
    typedef T* Pointer;

    inline explicit Allocator() throw() {}
    inline explicit Allocator(const Allocator& other) {}
    template<typename N>
    inline explicit Allocator(const Allocator<N>& other) {}

    inline Pointer Allocate(size_t size)
    {
      return static_cast<Pointer>(::operator new(size * sizeof(T)));
    }
    void Deallocate(Pointer ptr)
    {
      ::operator delete((void*)ptr);
    }
  };
}