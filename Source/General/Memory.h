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

  namespace Bit
  {
    template<size_t N, IntegralType T>
    static constexpr bool Get(T val)
    {
      return val & (1UL << N);
    }

    template<size_t N, IntegralType T>
    static constexpr T Apply(T val, bool bit)
    {
      return (val & ~(1UL << N)) | (bit << N);
    }

    template<size_t N, IntegralType T>
    static constexpr T Set(T val)
    {
      return val | (1UL << N);
    }

    template<size_t N, IntegralType T>
    static constexpr T Clear(T& val)
    {
      return val & ~(1UL << N);
    }

    template<size_t N, IntegralType T>
    static constexpr T Toggle(T& val)
    {
      return val ^ (1UL << N);
    }

    template<size_t N = 0, IntegralType T>
    static constexpr bool LSB(T val)
    {
      return val & (1UL << N);
    }

    template<size_t N = 0, typename T>
    static constexpr T ApplyLSB(T val, bool bit)
    {
      return (val & ~(1UL << N)) | (bit << N);
    }

    template<IntegralType T>
      requires (sizeof(T) > 0)
    static constexpr bool MSB(T val)
    {
        return val & (1UL << (sizeof(T) * 8 - 1UL));
    }

    template<IntegralType T>
      requires (sizeof(T) > 0)
    static constexpr T ApplyMSB(T val, bool bit)
    {
        return (val & ~(1UL << (sizeof(T) * 8 - 1UL))) | (bit << (sizeof(T) * 8 - 1UL));
    }
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
}