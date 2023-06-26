#pragma once

#include "General/Meta.h"

#include <exception>

namespace SSTD
{

  struct Exception
  {
    Exception() noexcept {}
    Exception(const char* msg) noexcept 
      : m_Msg(msg)
    {}

    virtual ~Exception();

    virtual const char* Message() const noexcept 
    { 
      return m_Msg; }
    const char* operator()() const noexcept { return Message(); }
  private:
    const char* m_Msg = "Unknown Exception";
  };

  struct IndexOutOfBounds : public Exception
  {
  public:
    IndexOutOfBounds(size_t index) :m_Index(index) , Exception("Index out of range") {}
  private:
    size_t m_Index = -1;
  };
}