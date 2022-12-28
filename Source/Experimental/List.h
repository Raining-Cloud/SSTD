#pragma once

#include "General/Numeric.h"
#include "General/Concepts.h"
#include "General/Allocator.h"
#include "Containers/Pair.h"
#include "Containers/Array.h"

namespace Experimental
{
  using namespace SSTD;
  template<typename T, IntegralType SizeType = size_t, template<typename> typename A = Allocator>
  class List
  {
    struct Node
    {
      T data;
      Node* next{ nullptr };
      Node* prev{ nullptr };
    };
  public:
    using AllocType = A<Node>;

    struct ListIterator
    {
      ListIterator(Node* n)
        : node(n)
      {}

      ListIterator& operator++() { node = node->next; }
      ListIterator& operator++(int) { node = node->next; }
      ListIterator& operator--() { node = node->prev; }
      ListIterator& operator--(int) { node = node->prev; }

      ListIterator& operator+=(SizeType distance)
      {
        for (SizeType i = 0; i < distance; ++i)
          operator++;
      }
      ListIterator& operator-=(SizeType distance)
      {
        for (SizeType i = 0; i < distance; ++i)
          operator--;
      }

      ListIterator operator+(SizeType distance) const
      {
        auto it = ListIterator(node);
        for (SizeType i = 0; i < distance; ++i)
          it++;
        return it;
      }
      ListIterator operator-(SizeType distance) const
      {
        auto it = ListIterator(node);
        for (SizeType i = 0; i < distance; ++i)
          it--;
        return it;
      }

      T* operator->() const { return *(node->data); }
      T& operator*() const { return node->data; }

      Node* node{ nullptr };
    };

    struct ConstListIterator
    {
      ConstListIterator(Node* n)
        : node(n)
      {}

      ConstListIterator& operator++() { node = node->next; }
      ConstListIterator& operator++(int) { node = node->next; }
      ConstListIterator& operator--() { node = node->prev; }
      ConstListIterator& operator--(int) { node = node->prev; }

      ConstListIterator& operator+=(SizeType distance)
      {
        for (SizeType i = 0; i < distance; ++i)
          operator++;
      }
      ConstListIterator& operator-=(SizeType distance)
      {
        for (SizeType i = 0; i < distance; ++i)
          operator--;
      }

      ConstListIterator operator+(SizeType distance) const
      {
        auto it = ConstListIterator(node);
        for (SizeType i = 0; i < distance; ++i)
          it++;
        return it;
      }
      ConstListIterator operator-(SizeType distance) const
      {
        auto it = ConstListIterator(node);
        for (SizeType i = 0; i < distance; ++i)
          it--;
        return it;
      }

      T* operator->() const { return *(node->data); }
      T& operator*() const { return node->data; }

      Node* node{ nullptr };
    };

    explicit List() {}

    List(const List& other)
      : m_Size(other.m_Size), m_Allocator(other.m_Allocator), m_StartSize(other.m_StartSize), m_EndSize(other.m_EndSize)
    {
      Node* nodes = m_Allocator.Allocate(other.m_Size);
      SizeType index = 1;
      for (const auto& v : other)
      {
        nodes[index]->prev = nodes[index - 1];
        nodes[index - 1]->next = nodes[index];
        m_Allocator.Construct(nodes[index - 1], v);
        ++index;
      }
    }

    List(List&& other) noexcept
    {
      Swap(m_Size, other.m_Size);
      Swap(m_StartNode, other.m_StartNode);
      Swap(m_EndNode, other.m_EndNode);
      Swap(m_Allocator, other.m_Allocator);
    }

    ~List() { Clear(); }

    List& operator=(const List& other)
    {
      Clear();

      Node* nodes = m_Allocator.Allocate(other.m_Size);
      SizeType index = 1;
      for (const auto& v : other)
      {
        nodes[index]->prev = nodes[index - 1];
        nodes[index - 1]->next = nodes[index];
        m_Allocator.Construct(nodes[index - 1], v);
        ++index;
      }

      return *this;
    }

    List& operator=(List&& other)
    {
      Clear();

      Swap(m_Size, other.m_Size);
      Swap(m_StartNode, other.m_StartNode);
      Swap(m_EndNode, other.m_EndNode);
      Swap(m_Allocator, other.m_Allocator);

      return *this;
    }

    T& operator[](SizeType index)
    {
      Node* it = m_StartNode;
      for (SizeType i = 0; i < index; ++i)
        it = it->next;

      return it->data;
    }

    T& const operator[](SizeType index) const
    {
      Node* it = m_StartNode;
      for (SizeType i = 0; i < index; ++i)
        it = it->next;

      return it->data;
    }

    void PushFront(const T& value) { EmplaceFront(value); }
    void PushFront(T&& value) { EmplaceFront(Forward<T>(value)); }

    template<typename ... Args>
    T& EmplaceFront(Args&&... args)
    {
      Node* node = m_Allocator.Allocate(1);
      m_Allocator.Construct(node->data, Forward<Args>(args)...);

      if (m_StartNode)
      {
        m_StartNode = node;
        m_EndNode = node;
      }
      else
      {
        node->next = m_StartNode;
        m_StartNode->prev = node;
        m_StartNode = node;
      }
      return m_StartNode->data;
    }

    void PushBack(const T& value) { EmplaceBack(value); }
    void PushBack(T&& value) { EmplaceBack(Forward<T>(value)); }

    template<typename ... Args>
    T& EmplaceBack(Args&&... args)
    {
      Node* node = m_Allocator.Allocate(1);
      m_Allocator.Construct(node->data, Forward<Args>(args)...);

      if (m_EndNode)
      {
        m_EndNode = node;
        m_StartNode = node;
      }
      else
      {
        node->prev = m_EndNode;
        m_EndNode->next = node;
        m_EndNode = node;
      }
      return m_EndNode->data;
    }

    void PopFront()
    {
      if (m_StartNode)
      {
        Node* start = m_StartNode->next;
        m_Allocator.Deallocate(m_StartNode);
        m_StartNode = start;
      }
    }
    void PopBack()
    {
      if (m_EndNode)
      {
        Node* end = m_EndNode->prev;
        m_Allocator.Deallocate(m_EndNode);
        m_EndNode = end;
      }
    }

    T& Front()
    {
      if (!m_StartNode)
        throw;
      return m_StartNode->data;
    }

    const T& Front() const
    {
      if (!m_StartNode)
        throw;
      return m_StartNode->data;
    }

    T& Back()
    {
      if (!m_EndNode)
        throw;

      return m_EndNode->data;
    }

    const T& Back() const
    {
      if (m_EndNode)
        return m_EndNode->data;
      throw;
    }

    T& At(SizeType index)
    {
      if (index > m_Size)
        throw;
      return operator[](index);
    }
    T& const At(SizeType index) const
    {
      if (index > m_Size)
        throw;
      return operator[](index);
    }

    ListIterator begin() { return ListIterator(m_StartNode); }
    ListIterator end() { return ListIterator(m_EndNode); }

    ConstListIterator cbegin() const { return ConstListIterator(m_StartNode); }
    ConstListIterator cend() const { return ConstListIterator(m_EndNode); }

    void Clear()
    {
      Node* cur = m_StartNode;
      while (cur != nullptr)
      {
        Node* next = cur->next;
        m_Allocator.Deallocate(cur);
        cur = next;
      }

      m_StartNode = nullptr;
      m_EndNode = nullptr;
      m_Size = 0;
    }

    SizeType Size() const { return m_Size; }
    SizeType Capacity() const { return 0; }
    bool IsEmpty() const { return m_Size == 0; }

  private:
    Node* m_StartNode{ nullptr };
    Node* m_EndNode{ nullptr };

    SizeType m_Size{ 0 };
    AllocType m_Allocator{};
  };
}