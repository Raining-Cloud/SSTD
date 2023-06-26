  #pragma once
#include "Numeric.h"
#include "Utility.h"

//#include <xmmintrin.h>

namespace SSTD
{
  using intptr = long long;
  using uintptr = unsigned long long;

  namespace Byte
  {
    static constexpr uint32 KB = 1024;
    static constexpr uint32 MB = 1048576;
    static constexpr uint32 GB = 1073741824;
    static constexpr uint64 TB = 1099511627776;
    static constexpr uint64 PB = 1125899906842624;
  };

  template<typename T>
  static constexpr size_t sizeof_b() { return sizeof(T) * 8; }

  struct Bit
  {
    uint8 value : 1;
  };

  template<size_t N = 0, typename T>
  static constexpr bool GetMSB(const T& value)
  {
    return (value >> sizeof_b<T>() - N - 1) & 1UL;
  }

  template<size_t N = 0, typename T>
  static constexpr void SetMSB(T& value, bool bit)
  {
    value = (value & ~(1UL << (sizeof_b<T>() - N - 1))) | (static_cast<T>(bit) << (sizeof_b<T>() - N - 1));
  }

  template<size_t N = 0, typename T>
  static constexpr bool GetLSB(const T& value)
  {
    return (value >> N) & 1UL;
  }

  template<size_t N = 0, typename T>
  static constexpr void SetLSB(T& value, bool bit)
  {
    value = (value & ~(1UL << N)) | (!!bit << N);
  }

  template<typename T>
    requires IsTriviallyCopyable<T>::valid
  static constexpr T* TMemCpy(T* dst, const T* src, size_t size)
  {
    //Copy bytewise is not th fastest but it will do the trick
    char* d = reinterpret_cast<char*>(dst);
    const char* s = reinterpret_cast<const char*>(src);

    for (size_t i = 0; i < sizeof(T)*size; ++i)
      d[i] = s[i];

    return dst;
  }

  template<typename T>
    requires (!IsTriviallyCopyable<T>::valid)
  static constexpr T* TMemCpy(T* dst, T* src, size_t size)
  {
    for (size_t i = 0; i < size; ++i)
    {
      T& val = src[i];
      new(dst + i) T(val);
    }
    return dst;
  }

  template<typename T>
    requires IsTriviallyCopyable<T>::valid
  static constexpr T* TMemMove(T* dst, T* src, size_t size)
  {
    char* d = reinterpret_cast<char*>(dst);
    const char* s = reinterpret_cast<const char*>(src);

    for (size_t i = 0; i < sizeof(T) * size; ++i)
    {
      d[i] = s[i];
      s[i] = 0x00;
    }
    return dst;
  }

  template<typename T>
    requires (!IsTriviallyCopyable<T>::valid)
  static constexpr T* TMemMove(T* dst, T* src, size_t size)
  {
    for (size_t i = 0; i < size; ++i)
    {
      T& val = src[i];
      new(dst + i) T(val);
      src[i] = T{};
    }
    return dst;
  }


  struct GenericGrow
  {
    static constexpr size_t Grow(size_t size)
    {
      return size * 1.66667;
    }
  };


  template<typename T>
  using Pointer = T*;

  
  template<typename T, IntegralType SizeType = size_t>
  struct MemoryBlock
  {
    operator bool() const { return (ptr != nullptr); }

    T* ptr = nullptr;
    SizeType size = 0L;
  };
}