#pragma once

#include "DefinePlatform.h"

#ifdef PLATFORM_WIN64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif