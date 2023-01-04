#pragma once
#include "Numeric.h"

namespace SSTD
{
  template<typename T, T v>
  struct Constant
  {

  };

  template<size_t SIZE, size_t MAX_SIZE>
  struct MaxSize {
    static constexpr bool valid = SIZE <= MAX_SIZE;
  };

  template<typename T>
  concept MaxSize8Bit = MaxSize<sizeof(T), 1>::valid;

  template<typename T>
  concept MaxSize16Bit = MaxSize<sizeof(T), 2>::valid;

  template<typename T>
  concept MaxSize32Bit = MaxSize<sizeof(T), 4>::valid;

  template<typename T>
  concept MaxSize64Bit = MaxSize<sizeof(T), 8>::valid;

  template<typename T>
  concept MaxSize128Bit = MaxSize<sizeof(T), 16>::valid;

  template <typename T>
  concept IntegralType = IsNumeric<T>::valid;

  template<typename T, typename U>
  struct IsSame { static constexpr bool valid = false; };

  template<typename T>
  struct IsSame<T, T> { static constexpr bool valid = true; };

  template<typename T, typename U>
  concept IsSameType = IsSame<T, U>::valid;

  template<typename T, typename U>
  concept IsDifferentType = !(IsSame<T, U>::valid);

  template <size_t A, size_t B>
  concept IsLess = (A < B);

  template<size_t A, size_t B>
  concept IsGreater = (A > B);

  template<size_t A, size_t B>
  concept IsLessEqual = (A <= B);

  template<size_t A, size_t B>
  concept IsGreaterEqual = (A >= B);

}
