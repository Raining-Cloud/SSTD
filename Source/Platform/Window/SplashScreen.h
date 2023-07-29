#pragma once

#include "Platform/IncludePlatform.h"

#include "Containers/String.h"

#include "Window.h"

namespace SSTD
{
  struct SplashScreenDesc
  {
    String image_path = "";

    float size = 0.3f; //relation of the current screen width

    bool alpha = false;
    Color8 alpha_color;
  };

  class SplashScreen
  {
  public:
    void Show();
    void Hide();

    void Close();

    static constexpr SplashScreen* Create(const SplashScreenDesc& desc) { return new SplashScreen(desc); }
    static constexpr void Release(SplashScreen* screen) { delete screen; }

  private:
    SplashScreen(const SplashScreenDesc& desc);
    ~SplashScreen();
#ifdef PLATFORM_WIN64
    HWND m_HWND = nullptr;
#elif PLATFORM_WIN32
#elif PLATFORM_IPHONE_SIM
#elif PLATFORM_IPHONE
#elif PLATFORM_MAC
#elif PLATFORM_LINUX
#endif
  };
}