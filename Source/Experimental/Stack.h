#pragma once
#include "General/Meta.h"

namespace SSTD 
{
  template<typename T, IntegralType SizeType>
  class Stack
  {
    struct Node
    {
      T* data[]
      Node* prev{ nullptr };
    };

  public:
    Stack() noexcept {}
    ~Stack() {}

    constexpr void Push(const T& value)
    {

    }

    constexpr void Pop()
    {
    
    }

    constexpr T& Peek() 
    {
      
    }

    void Reserve(SizeType size)
    {

    }

    constexpr SizeType Size() { return m_Size; }
    constexpr SizeType Capaity() { return m_Capacity; }

  private:

    Node* m_Cur{nullptr};

    SizeType m_Capacity{0};
    SizeType m_Size{0};
  };
}