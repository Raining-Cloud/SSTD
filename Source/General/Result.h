#pragma once

#include "Meta.h"
#include "Utility.h"

namespace SSTD
{
  template<typename T, typename E>
  struct Result
  {
    Result() noexcept :m_State(State::Error) {}
    Result(T value) :m_Val(value), m_State(State::Ok) {}
    Result(E error) :m_Err(error), m_State(State::Error) {}

    Result(const Result& other)
      : m_State(other.m_State)
    {
      if (other.Ok())
        m_Val = other.m_Val;
      else
        m_Err = other.m_Err;
    }

    Result(Result&& other) noexcept
      : m_State(other.m_State)
    {
      if (other.Ok())
        m_Val = Move(other.m_Val);
      else
        m_Err = Move(other.m_Err);
    }

    bool Ok() const noexcept { return m_State == State::Ok; }
    bool Error() const noexcept { return !Ok(); }

    operator bool() { return Ok(); }

    template<typename OkL, typename ErrL>
     requires IsInvokeable<OkL, T> && IsInvokeable<ErrL, E>
    auto Match(OkL&& ok, ErrL&& err) const
    {
      return Ok() ? ok(m_Val) : err(m_Err);
    }

    T* HasValue() const
    {
      return Ok() ? &m_Val : nullptr;
    }

  private:

    enum class State
    {
      Ok,
      Error
    };

    union
    {
      T m_Val;
      E m_Err{};
    };

    const State m_State = State::Error;
  };
}