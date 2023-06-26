#pragma once

#include "Numeric.h"

namespace SSTD
{
  template<typename T, T v>
  struct Constant
  {
    static constexpr T value = v;
    using Type = Constant;
    using ValueType = T;
    constexpr operator T() const noexcept { return value; }
    constexpr T operator()() const noexcept { return value; }
  };

  template<typename T, T a, T b>
  struct Min : public Constant<T, (a < b) ? a : b> {};

  template<typename T, T a, T b>
  struct Max : public Constant < T, (a > b) ? a : b> {};

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

  template <typename T>
  concept IsUnsigned = (T{ 0 } < T{ -1 });

  template<typename T, typename U>
  struct IsSame { static constexpr bool valid = false; };

  template<typename T>
  struct IsSame<T, T> { static constexpr bool valid = true; };

  template<typename T, typename U>
  concept IsSameType = IsSame<T, U>::valid;

  template<typename T, typename U>
  concept IsDifferentType = !(IsSame<T, U>::valid);

  template<typename T>
  concept IsAnyType = true;

  template<typename T, T a, T b>
  concept IsLess = (a < b);

  template<typename T, T a, T b>
  concept IsGreater = (a > b);

  template<typename T, T a, T b>
  concept IsLessEqual = (a <= b);

  template<typename T, T a, T b>
  concept IsGreaterEqual = (a >= b);

  template<typename T, template<typename> typename A>
  concept IsAllocator = requires(A<T> a, size_t n) {
    { a.Allocate(n) } -> IsSameType<T*>;
    { a.Deallocate(nullptr, n) } noexcept;
    { a.Construct(nullptr, T{}) } noexcept;
  };

  template<typename Lambda, typename ... Args>
  concept IsInvokeable = requires(Lambda l, Args&& ... args) {
    {l(args...)} -> IsAnyType;
  };
}
