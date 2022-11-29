#pragma once

#include "Platform/DefinePlatform.h"

namespace SSTD
{
  namespace Platform
  {
    class Console
    {
    public:
    private:
#ifdef PLATFORM_WIN64
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
    };
  }
}