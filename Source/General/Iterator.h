#pragma once


namespace SSTD
{
  template<typename T>
  class Iterator
  {
  public:
    using PointerType = T;
  public:
    Iterator() {}
    Iterator(T* ptr) :m_Ptr(ptr) {}

    bool operator!=(const Iterator& other) { return m_Ptr != other.m_Ptr; }
    bool operator==(const Iterator& other) { return m_Ptr == other.m_Ptr; }

    Iterator& operator++() { ++m_Ptr; return *this; }
    Iterator& operator--() { --m_Ptr; return *this; }
    Iterator operator++(int) { Iterator i(*this); ++m_Ptr; return i; }
    Iterator operator--(int) { Iterator i(*this); --m_Ptr; return i; }

    T& operator[] (int i) { return m_Ptr[i]; }

    T& operator*() { return *m_Ptr; }
    T* operator->() { return m_Ptr; }

    size_t operator-(const Iterator& other) { return m_Ptr - other.m_Ptr; }

  private:
    T* m_Ptr;
  };

  template<typename T>
  class ConstIterator
  {
  public:
    using PointerType = T;
  public:
    ConstIterator() {}
    ConstIterator(T* ptr) :m_Ptr(ptr) {}

    bool operator!=(const ConstIterator& other) { return m_Ptr != other.m_Ptr; }
    bool operator==(const ConstIterator& other) { return m_Ptr == other.m_Ptr; }

    ConstIterator& operator++() { ++m_Ptr; return *this; }
    ConstIterator& operator--() { --m_Ptr; return *this; }
    ConstIterator operator++(int) { ConstIterator i(*this); ++m_Ptr; return i; }
    ConstIterator operator--(int) { ConstIterator i(*this); --m_Ptr; return i; }

    const T& operator[](int i) const { return *(m_Ptr + i); }

    const T& operator*() const { return *m_Ptr; }
    const T* operator->() const { return m_Ptr; }

    size_t operator-(const ConstIterator& other) const { return m_Ptr - other.m_Ptr; }
  private:
    T* m_Ptr;
  };
}