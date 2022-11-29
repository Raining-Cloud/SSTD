#include "ConditionVariable.h"

namespace SSTD
{
#ifdef PLATFORM_WIN64
  ConditionVariable::ConditionVariable()
  {
    InitializeConditionVariable(&m_Handle);
  }
  ConditionVariable::~ConditionVariable()
  {
    NotifyAll();
  }
  void ConditionVariable::NotifyOne()
  {
    WakeConditionVariable(&m_Handle);
  }
  void ConditionVariable::NotifyAll()
  {
    WakeAllConditionVariable(&m_Handle);
  }
  void ConditionVariable::Wait(Lock<Mutex>& lock)
  {
    SleepConditionVariableCS(&m_Handle, (CRITICAL_SECTION*)&lock.GetLock().m_Handle, INFINITE);
  }
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
}