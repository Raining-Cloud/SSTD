#pragma once

#include "General/Pattern.h"
#include "General/Utility.h"
#include "General/Allocator.h"

namespace SSTD
{
  template<typename T, IntegralType SizeType = size_t>
  class SharedPointer
  {
     
  public:
    SharedPointer() :m_Ptr(nullptr), m_RefCount(nullptr) {}
    explicit SharedPointer(T* ptr) : m_Ptr(ptr), m_RefCount(new SizeType(1)) 
    {
      
    }
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

    ~SharedPointer() { Clear(); }

    void operator=(const SharedPointer& other) 
    {
      if (other.m_Ptr != m_Ptr)
      {
        Clear();
        m_Ptr = other.m_Ptr;
        m_RefCount = other.m_RefCount;
        (*m_RefCount)++;
      }
    }
    void operator=(SharedPointer&& other) 
    {
      if (other.m_Ptr != m_Ptr)
      {
        Clear();
        m_Ptr = Exchange(other.m_Ptr, nullptr);
        m_RefCount = Exchange(other.m_RefCount, nullptr);
      }
    }

    T* operator->() const { return m_Ptr; }
    T& operator*() const { return *(m_Ptr); }

    bool operator==(const T* ptr) { return m_Ptr == ptr; };
    bool operator!=(const T* ptr) { return !(*this == ptr);};

    bool operator==(const SharedPointer& other) { return m_Ptr == other.m_Ptr && m_RefCount == other.m_RefCount; }
    bool operator!=(const SharedPointer& other) { return !(*this == other); }

    explicit operator bool() const { return IsValid(); }

    T* Get() const noexcept { return m_Ptr; }

    T* Release()
    {
      (*m_RefCount)--;
      m_RefCount = nullptr;
      return Exchange(m_Ptr, nullptr);
    }

    void Clear() 
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

    bool IsValid() const { return m_Ptr != 0; }

    SizeType GetRefCount() const { return m_RefCount ? (*m_RefCount) : 0; }

  private:
    T* m_Ptr;
    SizeType* m_RefCount;
  };

  template<typename T>
  class UniquePointer : public NonCopyable
  {
  public:
    explicit UniquePointer() : m_Ptr(nullptr) {}
    UniquePointer(T* ptr) : m_Ptr(ptr) {}
    UniquePointer(UniquePointer&& other) noexcept : m_Ptr(other.m_Ptr) { other.m_Ptr = nullptr; }
    ~UniquePointer() { Clear(); }

    UniquePointer& operator=(UniquePointer&& other) noexcept
    {
      if (*this == other)
        return *this;

      Clear();

      m_Ptr = other.m_Ptr;
      other.m_Ptr = nullptr;

      return *this;
    }

    bool operator==(const T* ptr) { return m_Ptr == ptr; }
    bool operator!=(const T* ptr) { return !(*this == ptr); }

    bool operator==(const UniquePointer& other) { return m_Ptr == other.m_Ptr; }
    bool operator!=(const UniquePointer& other) { return !(*this == other); }

    T* operator->() const { return m_Ptr; }
    T& operator*() const { return *(m_Ptr); }

    explicit operator bool() const { return IsValid(); }

    T* Get() const noexcept { return m_Ptr; }

    T* Release()
    {
      return Exchange(m_Ptr, nullptr);
    }

    void Clear()
    {
      if (m_Ptr)
        delete m_Ptr;
      m_Ptr = nullptr;
    }

    bool IsValid() const { return m_Ptr != nullptr; }

  private:
    T* m_Ptr;
  };

  template<typename T, typename ... Args>
  static constexpr SharedPointer<T> MakeShared(Args&&... args)
  {
  }

  template<typename T, typename ... Args>
  static constexpr UniquePointer<T> MakeUnique(Args&&... args)
  {
  }
}