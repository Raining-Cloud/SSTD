#pragma once

#include "Platform/IncludePlatform.h"

namespace SSTD
{
  class Mutex
  {
  public:
    Mutex();
    ~Mutex();

    void Lock();
    bool TryLock();
    void Unlock();
    bool IsLocked();
#ifdef PLATFORM_WIN64
    friend class ConditionVariable;
    CRITICAL_SECTION m_Handle;
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
  private:
    unsigned int m_LockCount;
  };
}