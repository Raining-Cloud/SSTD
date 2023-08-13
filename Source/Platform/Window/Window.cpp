#include "Window.h"

namespace SSTD
{
  //////////////////////////////////////////////////////
  //                    GENERAL
  //////////////////////////////////////////////////////

  const Window::WindowEventQueue& Window::BeginEvents() const
  {
    return m_EventQueue;
  }

  void Window::EndEvents()
  {
    //Clear all the elements
    m_EventQueue.Erase();
  }

  //////////////////////////////////////////////////////
  //                    WINDOWS
  //////////////////////////////////////////////////////
#ifdef PLATFORM_WIN64
#include <windowsx.h>
#include <dwmapi.h>

#ifdef IsMinimized
#undef IsMinimized
#endif // IsMinimized

#ifdef IsMaximized
#undef IsMaximized
#endif // IsMaximized

  Window::Window(const WindowDesc& desc)
    : m_Desc(desc)
  {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEXA wndc = { 0 };
    wndc.cbSize = sizeof(WNDCLASSEX);
    wndc.style = CS_HREDRAW | CS_VREDRAW;
    wndc.lpfnWndProc = WndProcStatic;
    wndc.hInstance = hInstance;
    wndc.hCursor = LoadCursor(NULL, IDC_HAND);
    wndc.hIcon = (HICON)LoadImageA(NULL, m_Desc.icon_path.CStr(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_LOADTRANSPARENT);
    wndc.hIconSm = wndc.hIcon;
    wndc.lpszClassName = desc.name.CStr();

    if (!RegisterClassExA(&wndc))
    {
      if (GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
        throw;
    }

    m_HWND = CreateWindowExA(
      0,
      desc.name.CStr(),
      desc.title.CStr(),
      ToNative(m_Desc.window_style),
      m_Desc.x,
      m_Desc.y,
      m_Desc.width,
      m_Desc.height,
      NULL,
      NULL,
      hInstance,
      this); //this can cause trouble while moving!
    
    if (!m_HWND) return; //failed to create the window!

    SetTitleBarColor(m_Desc.titlebar_color);
  }

  Window::~Window()
  {
    if (m_HWND != nullptr)
      Close();
  }

  void Window::Procces()
  {
    if (m_HWND == nullptr)
      return;

    MSG msg = { };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //needs a bit more of the CPU but also is faster in framerate!
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  void Window::Realign(uint32 width, uint32 height, uint32 x, uint32 y)
  {
    SetWindowPos(m_HWND, 0, x, y, width, height, SWP_NOZORDER | SWP_NOSIZE);
  }

  void Window::SetPosition(uint32 x, uint32 y)
  {
    SetWindowPos(m_HWND, 0, x, y, m_Desc.width, m_Desc.height, SWP_NOZORDER | SWP_NOSIZE);
  }

  Vec2<uint32> Window::GetPosition() const
  {
    RECT r;
    GetWindowRect(m_HWND, &r);
    return {r.left, r.top};
  }


  void Window::SetSize(uint32 width, uint32 height)
  {
    RECT r;
    if (GetWindowRect(m_HWND, &r))
    {
      SetWindowPos(m_HWND, 0, r.left, r.top, width, height, 0);
    }
  }

  Vec2<uint32> Window::GetSize() const
  {
    RECT r;
    GetWindowRect(m_HWND, &r);
    return { r.right - r.left, r.bottom - r.top };
  }

  void Window::SetTitle(const SSTD::String& title)
  {
    SetWindowTextA(m_HWND, title.CStr());
  }

  String Window::GetTitle() const
  {
    uint32 len = GetWindowTextLengthA(m_HWND);
    String s;
    s.Resize(len);

    GetWindowTextA(m_HWND, s.Data(), len);
    return s;
  }

  void Window::SetBackgroundColor(const Color8& col)
  {
    m_Desc.background_color = col;
  }

  Color8 Window::GetBackgroundColor()
  {
    return m_Desc.background_color;
  }

  void Window::SetTitleBarColor(const Color8& col)
  {
    COLORREF colref = RGB(col.r, col.g, col.b);
    DwmSetWindowAttribute(m_HWND, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &colref, sizeof(colref));
  }

  Color8 Window::GetTitleBarColor()
  {
    COLORREF col;
    DwmGetWindowAttribute(m_HWND, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &col, sizeof(col));
    return { GetRValue(col), GetGValue(col), GetBValue(col) };
  }

  void* Window::GetNative() const
  {
    return static_cast<void*>(m_HWND);
  }

  void Window::Close()
  {
    if (m_HWND != nullptr)
    {
      DestroyWindow(m_HWND);
    }
  }

  void Window::Minimize()
  {
    ShowWindow(m_HWND, SW_MINIMIZE);
  }

  bool Window::IsMinimized() const
  {
    return IsIconic(m_HWND);
  }

  void Window::Maximize()
  {
    ShowWindow(m_HWND, SW_MAXIMIZE);
  }

  bool Window::IsMaximized() const
  {
    return IsZoomed(m_HWND);
  }

  void Window::Hide()
  {
    ShowWindow(m_HWND, SW_HIDE);
  }

  bool Window::IsHidden()
  {
    return !IsVisible();
  }

  void Window::Show()
  {
    ShowWindow(m_HWND, SW_SHOW);
  }

  bool Window::IsVisible() const
  {
    return IsWindowVisible(m_HWND);
  }

  void Window::Focus()
  {
    SetFocus(m_HWND);
  }

  bool Window::IsFocused() const
  {
    return (m_HWND == GetFocus());
  }

  void Window::Fullscreen(bool state)
  {
    //cheack wheater we need to switch the state!
    DWORD dwStyle = GetWindowLong(m_HWND, GWL_STYLE);
    if (dwStyle & WS_OVERLAPPEDWINDOW && state)
    {
      //set to fullscreen
      MONITORINFO mi = { sizeof(mi) };
      WINDOWPLACEMENT wpl;
      if (GetWindowPlacement(m_HWND, &wpl) && GetMonitorInfo(MonitorFromWindow(m_HWND, MONITOR_DEFAULTTOPRIMARY), &mi))
      {
        SetWindowPos(m_HWND, HWND_TOP,
          mi.rcMonitor.left, mi.rcMonitor.top,
          mi.rcMonitor.right - mi.rcMonitor.left,
          mi.rcMonitor.bottom - mi.rcMonitor.top,
          SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowLong(m_HWND, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW); //looks cleaner becaus the user cant see the style change
      }
    }
    else
    {
      //set to windowed
      SetWindowLong(m_HWND, GWL_STYLE, ToNative(m_Desc.window_style));
      //SetWindowPlacement(m_WindowHandle, &m_WindowPlacement); //ToDo: implement this to keep original position and size
      SetWindowPos(m_HWND, NULL, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
        SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

      ShowWindow(m_HWND, SW_NORMAL);
    }
  }

  bool Window::IsFullscreen() const
  {
    RECT r, wr;
    GetWindowRect(GetDesktopWindow(), &r);
    GetWindowRect(m_HWND, &wr);
    return r.right == (wr.right - wr.left) && r.bottom == (wr.bottom - wr.top);
  }

  void Window::Center()
  {
    const auto dim = GetSize();
    SetPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - dim.x() / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - dim.y() / 2);
  }

  LRESULT Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    Window* pThis = nullptr;
    if (msg == WM_CREATE)
    {
      CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
      pThis = (Window*)pCreate->lpCreateParams;
      SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else
      pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (pThis)
      return pThis->WndProc(hwnd, msg, wParam, lParam);
    else
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }

  LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    WindowEvent e;
    switch (msg)
    {
    case WM_CREATE:
      e = WindowEvent(EventType::Create);
      break;
    case WM_DESTROY:
    case WM_CLOSE:
      e = WindowEvent(EventType::Close);
      break;
    case WM_PAINT:
    {
      e = WindowEvent(EventType::Paint);
      break;
    }
    case WM_SIZE:
    {
      uint32 w, h;
      w = static_cast<uint32>(LOWORD(lParam));
      h = static_cast<uint32>(HIWORD(lParam));
      e = WindowEvent(ResizeData{ .width = w, .height = h });
      break;
    }
    case WM_GETMINMAXINFO:
    {
      LPMINMAXINFO lMinMax = (LPMINMAXINFO)lParam;
      lMinMax->ptMinTrackSize.x = m_Desc.min_width;
      lMinMax->ptMinTrackSize.y = m_Desc.min_height;
      break;
    }
    case WM_DPICHANGED:
    {
      float scale = static_cast<float>(HIWORD(wParam) / USER_DEFAULT_SCREEN_DPI);
      e = WindowEvent(DPIData{ .scale = scale });
      break;
    }
    case WM_SETFOCUS:
      e = WindowEvent(FocusData{ .is_focused = true });
      break;
    case WM_KILLFOCUS:
      e = WindowEvent(FocusData{ .is_focused = false });
      break;
    case WM_MOVE:
    {
      e = WindowEvent(MoveData{ .x = static_cast<uint32>(LOWORD(lParam)), .y = static_cast<uint32>(HIWORD(lParam)) });
      break;
    }
    case WM_MOUSEWHEEL:
      e = WindowEvent(MouseWheelData{ .delta = GET_WHEEL_DELTA_WPARAM(wParam) });
      break;
    case WM_MOUSEMOVE:
    {
      e = WindowEvent(MouseMoveData{ .x = static_cast<int32>(LOWORD(lParam)), .y = static_cast<int32>(HIWORD(lParam)) });
      break;
    }
    case WM_LBUTTONDOWN:
      e = WindowEvent(MouseInputData{ .key = MouseCode::LeftMouse, .state = ButtonState::Pressed, .x = GET_X_LPARAM(lParam), .y = GET_Y_LPARAM(lParam) });
      break;
    case WM_LBUTTONUP:
      e = WindowEvent(MouseInputData{ .key = MouseCode::LeftMouse, .state = ButtonState::Released, .x = GET_X_LPARAM(lParam), .y = GET_Y_LPARAM(lParam) });
      break;
    case WM_RBUTTONDOWN:
      e = WindowEvent(MouseInputData{ .key = MouseCode::RightMouse, .state = ButtonState::Pressed, .x = GET_X_LPARAM(lParam), .y = GET_Y_LPARAM(lParam) });
      break;
    case WM_RBUTTONUP:
      e = WindowEvent(MouseInputData{ .key = MouseCode::RightMouse, .state = ButtonState::Released, .x = GET_X_LPARAM(lParam), .y = GET_Y_LPARAM(lParam) });
      break;
    case WM_MBUTTONDOWN:
      e = WindowEvent(MouseInputData{ .key = MouseCode::MiddleMouse, .state = ButtonState::Pressed, .x = GET_X_LPARAM(lParam), .y = GET_Y_LPARAM(lParam) });
      break;
    case WM_MBUTTONUP:
      e = WindowEvent(MouseInputData{ .key = MouseCode::MiddleMouse, .state = ButtonState::Released, .x = GET_X_LPARAM(lParam), .y = GET_Y_LPARAM(lParam) });
      break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
      e = WindowEvent(KeyboardInputData{ .key = ToKeyCode(static_cast<uint8>(wParam)), .state = ButtonState::Pressed });
      break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
      e = WindowEvent(KeyboardInputData{ .key = ToKeyCode(static_cast<uint8>(wParam)), .state = ButtonState::Released });
      break;
    case WM_CHAR:
      e = WindowEvent(CharData{ .c = static_cast<char>(wParam) });
      break;
    default:
      break;
    }
    //only push if we have an actual event!
    if (e.Type() != EventType::None)
    {
      m_EventQueue.EmplaceBack(e);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
  }

  DWORD Window::ToNative(WindowStyle style) const
  {
    switch (style)
    {
    case WindowStyle::Borderless:
      return WS_POPUP | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    default:
      return WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX; // use normal style as default!
    }
  }

#elif PLATFORM_WIN32
#elif PLATFORM_IPHONE_SIM
#elif PLATFORM_IPHONE
#elif PLATFORM_MAC
#elif PLATFORM_LINUX
#endif 
}
