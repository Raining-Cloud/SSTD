#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
//define something for Windows (32-bit and 64-bit, this part is General)
#ifdef _WIN64
  //define something for Windows (64-bit only)
#define PLATFORM_WIN64 1
#else
  //define something for Windows (32-bit only)
#define PLATFORM_WIN32 1
#error "Windows 32-bit is not supported"
#endif
#elif __APPLE__
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define PLATFORM_IPHONE_SIM 1
#error "IOS Simulator is not supported"
#elif TARGET_OS_IPHONE
// iOS device
#define PLATFORM_IPHONE 1
#error "iPhone is not supported"
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#define PLATFORM_MAC 1
#error "Mac is not supported"
#else
#error "Unknown Apple platform"
#endif
#elif __linux__
// linux
#define PLATFORM_LINUX 1
#error "Linux is not supported"
#else
#error "Unknown compiler"
#endif

/*
* TEMPLATE
#ifdef PLATFORM_WIN64
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
*/