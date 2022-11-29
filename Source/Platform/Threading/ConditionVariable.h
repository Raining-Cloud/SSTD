#pragma once

#include "Platform/IncludePlatform.h"

#include "Lock.h"
#include "Mutex.h"

namespace SSTD
{

  class ConditionVariable
  {
  public:
    ConditionVariable();
    ~ConditionVariable();

    void NotifyOne();
    void NotifyAll();

    void Wait(Lock<Mutex>& lock);

    template<typename Check>
    void WaitFor(Lock<Mutex>& lock, Check&& check)
    {
      while (!check())
        Wait(lock);
    }
    //virtual void waitUntil(Lock<Mutex>& lock) = 0;

  private:
#ifdef PLATFORM_WIN64
    CONDITION_VARIABLE m_Handle;
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
  };
}