#pragma once

#include "Platform/DefinePlatform.h"
#include "Containers/Function.h"

#include "General/Numeric.h"

namespace SSTD
{
  enum class ThreadState
  {
    Created,
    Running,
    Finished
  };

  class Thread
  {
  public:
    Thread(const Function<void()>& func, bool launch) noexcept;
    virtual ~Thread();

    void Start();
    void Join();
    void Detach();

    void* GetNative();
  private:
#ifdef PLATFORM_WIN64
    void* m_Handle = nullptr;
    uint32 m_Id;
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
  };
}