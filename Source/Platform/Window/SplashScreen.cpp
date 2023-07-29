#include "SplashScreen.h"

#include "Math/Math.h"

namespace SSTD
{
#ifdef PLATFORM_WIN64
  SplashScreen::SplashScreen(const SplashScreenDesc& desc)
  {
    HBITMAP hbm = static_cast<HBITMAP>(LoadImageA(NULL, desc.image_path.CStr(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
    if (!hbm)
      return; //failed to load the image

    BITMAP bm;
    GetObject(hbm, sizeof(BITMAP), &bm);

    //get the screen size
    RECT r;
    GetWindowRect(GetDesktopWindow(), &r);

    uint32 width = r.right * Math::Clamp(desc.size, 0.f, 1.f); //calculate the width of the actual splash
    uint32 height = width * (bm.bmHeight / static_cast<float>(bm.bmWidth));

    m_HWND = CreateWindowExA(
      WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
      "STATIC",
      "SplashScreen",
      SS_REALSIZECONTROL | SS_BITMAP | WS_POPUP,
      (r.right / 2) - (width / 2),
      (r.bottom / 2) - (height / 2),
      width,
      height,
      NULL,
      NULL,
      NULL,
      this);

    if (!m_HWND)
      return; //failed to create window

    //set the splash-screen image
    SendMessage(m_HWND, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm);

    //check if we have alpha enabled
    if (desc.alpha)
    {
      SetWindowLong(m_HWND, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
      SetLayeredWindowAttributes(m_HWND, RGB(desc.alpha_color.r, desc.alpha_color.g, desc.alpha_color.b), 0, LWA_COLORKEY);
    }
  }

  SplashScreen::~SplashScreen()
  {
    if (m_HWND)
      Close();
  }

  void SplashScreen::Show()
  {
    if (m_HWND)
      ShowWindow(m_HWND, SW_SHOW);
  }

  void SplashScreen::Hide()
  {
    if (m_HWND)
      ShowWindow(m_HWND, SW_HIDE);
  }

  void SplashScreen::Close()
  {
    if (m_HWND)
      DestroyWindow(m_HWND);
  }
#elif PLATFORM_WIN32
#elif PLATFORM_IPHONE_SIM
#elif PLATFORM_IPHONE
#elif PLATFORM_MAC
#elif PLATFORM_LINUX
#endif
  }