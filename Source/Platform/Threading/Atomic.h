#pragma once

#include "Platform/IncludePlatform.h"

#include "General/Numeric.h"
#include "General/Concepts.h"
#include "AtomicUtils.h"

namespace SSTD
{
  template<IntegralType T, size_t = sizeof(T)>
  class AtomicInt;

  template<IntegralType T>
  class AtomicInt<T, 1>
  {
    using DataType = NumericTypeFromSize<sizeof(T)>::Unsigned;
  public:
    AtomicInt() : m_Data(0) {}
    AtomicInt(T value) : m_Data(static_cast<uint8>(value)) {}

    constexpr void Store(T value)
    {
      AtomicUtils::Store(m_Data, static_cast<DataType>(value));
    }

    constexpr T Load()
    {
      return static_cast<T>(AtomicUtils::Load(m_Data));
    }

    operator T()
    {
      return static_cast<T>(AtomicUtils::Load(m_Data));
    }

    void operator=(T value)
    {
      Store(value);
    }

    T operator++()
    {
      return static_cast<T>(AtomicUtils::PreIncrement(m_Data));
    }

    T operator++(int)
    {
      return static_cast<T>(AtomicUtils::PostIncrement(m_Data));
    }

    T operator--()
    {
      return static_cast<T>(AtomicUtils::PreDecrement(m_Data));
    }

    T operator--(int)
    {
      return static_cast<T>(AtomicUtils::PostDecrement(m_Data));
    }

    T operator+=(T value)
    {
      return static_cast<T>(AtomicUtils::Add(m_Data, static_cast<DataType>(value)));
    }

    T operator-=(T value)
    {
      return static_cast<T>(AtomicUtils::Subtract(m_Data, static_cast<DataType>(value)));
    }

    T operator&=(T value)
    {
      return static_cast<T>(AtomicUtils::And(m_Data, static_cast<DataType>(value)));
    }

    T operator|=(T value)
    {
      return static_cast<T>(AtomicUtils::Or(m_Data, static_cast<DataType>(value)));
    }

    T operator^=(T value)
    {
      return static_cast<T>(AtomicUtils::Xor(m_Data, static_cast<DataType>(value)));
    }
  private:
      volatile DataType m_Data;
  };

  using a_int8 = AtomicInt<int8>;
  using a_int16 = AtomicInt<int16>;
  using a_int32 = AtomicInt<int32>;
  using a_int64 = AtomicInt<int64>;

  using a_uint8 = AtomicInt<uint8>;
  using a_uint16 = AtomicInt<uint16>;
  using a_uint32 = AtomicInt<uint32>;
  using a_uint64 = AtomicInt<uint64>;

  class AtomicBool
  {
  public:
    AtomicBool() : m_Data(0) {};
    AtomicBool(bool value) : m_Data(static_cast<uint8>(value)) {};
    AtomicBool(const AtomicBool& other) : m_Data(other.m_Data) {};

    void Store(bool value)
    {
      m_Data.Store(static_cast<uint8>(value));
    }

    bool Load()
    {
      return m_Data.Load();
    }

    void operator=(const AtomicBool& other)
    {
      m_Data = other.m_Data;
    }

    void operator=(bool value) 
    {
      m_Data.Store(static_cast<uint8>(value));
    }

    operator bool() 
    {
      return m_Data.Load() == 1;
    };
  private:
    a_int8 m_Data;
  };
  using a_bool = AtomicBool;
}