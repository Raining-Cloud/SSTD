#include "Mutex.h"

namespace SSTD
{
#ifdef PLATFORM_WIN64
  Mutex::Mutex() :m_LockCount(0)
  {
    InitializeCriticalSection(&m_Handle);
  }
  Mutex::~Mutex()
  {
    DeleteCriticalSection(&m_Handle);
  }
  void Mutex::Lock()
  {
    EnterCriticalSection(&m_Handle);
    ++m_LockCount;
  }
  bool Mutex::TryLock()
  {
    if (TryEnterCriticalSection(&m_Handle))
    {
      return true;
      ++m_LockCount;
    }
    return false;
  }
  void Mutex::Unlock()
  {
    --m_LockCount;
    LeaveCriticalSection(&m_Handle);
  }
  bool Mutex::IsLocked()
  {
    return m_LockCount > 0;
  }
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
}