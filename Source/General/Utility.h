#pragma once

#include "Concepts.h"

namespace SSTD
{
  template<class T>
  struct RemoveReference
  {
    using Type = T;
  };

  template<class T>
  struct RemoveReference<T&>
  {
    using Type = T;
  };

  template<class T>
  struct RemoveReference<T&&>
  {
    using Type = T;
  };

  template<typename T>
  struct IsTriviallyCopyable
  {
    static constexpr bool valid = __is_trivially_copyable(T);
  };

  template<typename T>
  struct IsPointer
  {
    static constexpr bool valid = false;
  };

  template<typename T>
  struct IsPointer<T*>
  {
    static constexpr bool valid = true;
  };

  template <class T>
  static constexpr T&& Forward(typename RemoveReference<T>::Type& t) noexcept
  {
    return static_cast<T&&>(t);
  }

  template <class T>
  static constexpr T&& Forward(typename RemoveReference<T>::Type&& t) noexcept
  {
    return static_cast<T&&>(t);
  }

  template<typename T>
  static constexpr void Swap(T& t1, T& t2) noexcept
  {
    auto temp = Move(t1);
    t1 = Move(t2);
    t2  = Move(temp);
  }

  template<typename T, typename U>
  static constexpr T Exchange(T& obj, U&& val)
  {
    auto out = Move(obj);
    obj = Move(val);
    return out;
  }

  template<typename T>
  static constexpr typename RemoveReference<T>::Type&& Move(T&& arg) noexcept
  {
    return static_cast<typename RemoveReference<T>::Type&&>(arg);
  }
}