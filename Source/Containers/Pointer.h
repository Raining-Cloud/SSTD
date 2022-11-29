#pragma once

#include "General/Pattern.h"
#include "General/Utility.h"

namespace SSTD
{
  template<typename T, IntegralType SizeType = size_t>
  class SharedPointer
  {
  public:
    SharedPointer() :m_Ptr(nullptr), m_RefCount(nullptr) {}
    SharedPointer(T* ptr) : m_Ptr(ptr), m_RefCount(new SizeType(1)) {}
    SharedPointer(const SharedPointer& other)
      : m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount)
    {
      (*m_RefCount)++;
    }

    SharedPointer(SharedPointer&& other)
      : m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount)
    {
      other.m_Ptr = nullptr;
      other.m_RefCount = nullptr;
    }

    template<typename ... Args>
    SharedPointer(Args... args)
      : m_RefCount(new SizeType(1))
    {
      m_Ptr = new T(args...);
    }

    ~SharedPointer() { Clear(); }

    void operator=(const SharedPointer& other) 
    { 
      Clear();
      m_Ptr = other.m_Ptr;
      m_RefCount = other.m_RefCount;
      (*m_RefCount)++;
    }
    void operator=(SharedPointer&& other) 
    {
      Clear();
      m_Ptr = Exchange(other.m_Ptr, nullptr);
      m_RefCount = Exchange(other.m_RefCount, nullptr);
    }

    T* operator->() const { return m_Ptr; }
    T& operator*() const { return *(m_Ptr); }

    explicit operator bool() const { return IsValid(); }

    inline T* Get() const noexcept { return m_Ptr; }

    inline T* Release()
    {
      (*m_RefCount)--;
      m_RefCount = nullptr;
      return Exchange(m_Ptr, nullptr);
    }

    inline void Clear() 
    {
      if (m_Ptr)
      {
        (*m_RefCount)--;
        if (m_RefCount == 0)
        {
          delete m_Ptr;
          delete m_RefCount;
        }
        m_Ptr = nullptr;
        m_RefCount = nullptr;
      }
    }

    constexpr bool IsValid() const { return m_Ptr != 0; }

    inline SizeType GetRefCount() const { return m_RefCount ? (*m_RefCount) : 0; }

  private:

    T* m_Ptr;
    SizeType* m_RefCount;
  };

  template<typename T>
  class UniquePointer : NonCopyable
  {
  public:
    explicit UniquePointer() : m_Ptr(nullptr) {}
    UniquePointer(T* ptr) : m_Ptr(ptr) {}
    UniquePointer(UniquePointer&& other) noexcept : m_Ptr(other.m_Ptr) { other.m_Ptr = nullptr; }

    template<typename ... Args>
    UniquePointer(Args ... args) { m_Ptr = new T(Forward<Args...>(args...)); }

    ~UniquePointer() { Clear(); }

    UniquePointer& operator=(UniquePointer&& other) noexcept
    {
      if (this == other)
        return *this;

      Clear();

      m_Ptr = other.m_Ptr;
      other.m_Ptr = nullptr;
    }

    T* operator->() const { return m_Ptr; }
    T& operator*() const { return *(m_Ptr); }

    explicit operator bool() const { return IsValid(); }

    inline T* Get() const noexcept { return m_Ptr; }

    inline T* Release()
    {
      return Exchange(m_Ptr, nullptr);
    }

    void Clear()
    {
      if (m_Ptr)
        delete m_Ptr;
      m_Ptr = nullptr;
    }

    constexpr bool IsValid() const { return m_Ptr != nullptr; }

  private:
    T* m_Ptr;
  };
}