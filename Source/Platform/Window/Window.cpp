#include "Window.h"

namespace SSTD
{
  //////////////////////////////////////////////////////
//                    GENERAL
//////////////////////////////////////////////////////

  bool Window::PollEvents(WindowEvent& e)
  {
    if (!m_EventQueue.empty())
    {
      e = m_EventQueue.front();
      m_EventQueue.pop();
      return true;
    }
    return false;
  }

  //////////////////////////////////////////////////////
  //                    WINDOWS
  //////////////////////////////////////////////////////
#ifdef PLATFORM_WIN64
#include <windowsx.h>

#ifdef IsMinimized
#undef IsMinimized
#endif // IsMinimized

#ifdef IsMaximized
#undef IsMaximized
#endif // IsMaximized

  Window::Window()
    :m_Desc(), m_WindowHandle(nullptr), m_EventQueue(), m_WindowPlacement()
  {}

  Window::Window(const WindowDesc& desc)
    : m_Desc(desc)
  {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEXA wndc = { 0 };
    wndc.cbSize = sizeof(WNDCLASSEX);
    wndc.style = CS_HREDRAW | CS_VREDRAW;
    wndc.lpfnWndProc = WndProcStatic;
    wndc.hInstance = hInstance;
    wndc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndc.lpszClassName = desc.title.CStr();

    if (!RegisterClassExA(&wndc))
    {
      if (GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
        throw;
    }

    m_WindowHandle = CreateWindowExA(
      0,
      desc.title.CStr(),
      desc.name.CStr(),
      ToNative(m_Desc.window_style),
      m_Desc.x,
      m_Desc.y,
      m_Desc.width,
      m_Desc.height,
      nullptr,
      nullptr,
      hInstance,
      this); //this can cause trouble while moving!

    if (m_Desc.visible)
    {
      Show();
      Focus();
      if (m_Desc.centered)
        Center();
      Fullscreen(m_Desc.fullscreen);
    }
  }

  Window::~Window()
  {
    if (m_WindowHandle != nullptr)
      Close();
  }

  void Window::Procces()
  {
    if (m_WindowHandle == nullptr)
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
    SetWindowPos(m_WindowHandle, 0, x, y, width, height, SWP_NOZORDER | SWP_NOSIZE);
  }

  void Window::SetPosition(uint32 x, uint32 y)
  {
    SetWindowPos(m_WindowHandle, 0, x, y, m_Desc.width, m_Desc.height, SWP_NOZORDER | SWP_NOSIZE);
    m_Desc.x = x;
    m_Desc.y = y;
  }

  void Window::SetSize(uint32 width, uint32 height)
  {
    RECT r;
    if (GetWindowRect(m_WindowHandle, &r))
    {
      SetWindowPos(m_WindowHandle, 0, r.left, r.top, width, height, 0);
      m_Desc.x = r.left;
      m_Desc.y = r.top;
    }
    m_Desc.width = width;
    m_Desc.height = height;
  }

  void Window::SetTitle(const SSTD::String& title)
  {
    SetWindowTextA(m_WindowHandle, title.CStr());
    m_Desc.title = title;
  }

  void* Window::GetNative()
  {
    return static_cast<void*>(m_WindowHandle);
  }

  void Window::Close()
  {
    if (m_WindowHandle != nullptr)
      DestroyWindow(m_WindowHandle);
  }

  void Window::Minimize()
  {
    ShowWindow(m_WindowHandle, SW_MINIMIZE);
  }

  bool Window::IsMinimized() const
  {
    return IsIconic(m_WindowHandle);
  }

  void Window::Maximize()
  {
    ShowWindow(m_WindowHandle, SW_MAXIMIZE);
  }

  bool Window::IsMaximized() const
  {
    return IsZoomed(m_WindowHandle);
  }

  void Window::Hide()
  {
    ShowWindow(m_WindowHandle, SW_HIDE);
  }

  bool Window::IsHidden()
  {
    return !IsVisible();
  }

  void Window::Show()
  {
    ShowWindow(m_WindowHandle, SW_SHOW);
  }

  bool Window::IsVisible() const
  {
    return IsWindowVisible(m_WindowHandle);
  }

  void Window::Focus()
  {
    SetFocus(m_WindowHandle);
  }

  bool Window::IsFocused() const
  {
    return (m_WindowHandle == GetFocus());
  }

  void Window::Fullscreen(bool state)
  {
    //cheack wheater we need to switch the state!
    DWORD dwStyle = GetWindowLong(m_WindowHandle, GWL_STYLE);
    if (dwStyle & WS_OVERLAPPEDWINDOW && state)
    {
      //set to fullscreen
      MONITORINFO mi = { sizeof(mi) };
      if (GetWindowPlacement(m_WindowHandle, &m_WindowPlacement) && GetMonitorInfo(MonitorFromWindow(m_WindowHandle, MONITOR_DEFAULTTOPRIMARY), &mi))
      {
        SetWindowPos(m_WindowHandle, HWND_TOP,
          mi.rcMonitor.left, mi.rcMonitor.top,
          mi.rcMonitor.right - mi.rcMonitor.left,
          mi.rcMonitor.bottom - mi.rcMonitor.top,
          SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowLong(m_WindowHandle, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW); //looks cleaner becaus the user cant see the style change
      }
    }
    else
    {
      //set to windowed
      SetWindowLong(m_WindowHandle, GWL_STYLE, ToNative(m_Desc.window_style));
      SetWindowPlacement(m_WindowHandle, &m_WindowPlacement);
      SetWindowPos(m_WindowHandle, NULL, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
        SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

      ShowWindow(m_WindowHandle, SW_NORMAL);
    }
  }

  bool Window::IsFullscreen() const
  {
    return m_Desc.fullscreen;
  }

  void Window::Center()
  {
    SetPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - m_Desc.width / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - m_Desc.height / 2);
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
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      HBRUSH col = CreateSolidBrush(RGB(m_Desc.background_color.r, m_Desc.background_color.g, m_Desc.background_color.b));
      FillRect(hdc, &ps.rcPaint, col);
      EndPaint(hwnd, &ps);
      DeleteObject(col);
      e = WindowEvent(EventType::Paint);
      break;
    }
    case WM_SIZE:
    {
      uint32 w, h;
      w = static_cast<uint32>(LOWORD(lParam));
      h = static_cast<uint32>(HIWORD(lParam));
      m_Desc.width = w; m_Desc.height = h;
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
      uint32 x, y;
      x = static_cast<uint32>(LOWORD(lParam));
      y = static_cast<uint32>(HIWORD(lParam));
      m_Desc.x = x; m_Desc.y = y;
      e = WindowEvent(EventType::Move);
      break;
    case WM_MOUSEHWHEEL: //STODO: THIS IS NOT WORKING FIX NEEDED!!!
      e = WindowEvent(MouseWheelData{ .delta = GET_WHEEL_DELTA_WPARAM(wParam) });
      break;
    case WM_MOUSEMOVE:
    {
      int x = static_cast<short>(LOWORD(lParam));
      int y = static_cast<short>(HIWORD(lParam));
      e = WindowEvent(MouseMoveData{ .position = Vec2<uint32>(x,y) });
      break;
    }
    case WM_LBUTTONDOWN:
      e = WindowEvent(MouseInputData{ .key = MouseCode::LeftMouse, .state = ButtonState::Pressed, .position = Vec2<uint32>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) });
      break;
    case WM_LBUTTONUP:
      e = WindowEvent(MouseInputData{ .key = MouseCode::LeftMouse, .state = ButtonState::Released, .position = Vec2<uint32>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) });
      break;
    case WM_RBUTTONDOWN:
      e = WindowEvent(MouseInputData{ .key = MouseCode::RightMouse, .state = ButtonState::Pressed, .position = Vec2<uint32>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) });
      break;
    case WM_RBUTTONUP:
      e = WindowEvent(MouseInputData{ .key = MouseCode::RightMouse, .state = ButtonState::Released, .position = Vec2<uint32>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) });
      break;
    case WM_MBUTTONDOWN:
      e = WindowEvent(MouseInputData{ .key = MouseCode::MiddleMouse, .state = ButtonState::Pressed, .position = Vec2<uint32>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) });
      break;
    case WM_MBUTTONUP:
      e = WindowEvent(MouseInputData{ .key = MouseCode::MiddleMouse, .state = ButtonState::Released, .position = Vec2<uint32>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) });
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
      m_EventQueue.push(e);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
  }

  DWORD Window::ToNative(WindowStyle style) const
  {
    switch (m_Desc.window_style)
    {
    case WindowStyle::Borderless:
      return WS_POPUPWINDOW;
    case WindowStyle::PopUp:
      return WS_SYSMENU;
    default:
      return WS_OVERLAPPEDWINDOW; // use normal style as default!
    }
  }

#elif PLATFORM_WIN32
#elif PLATFORM_IPHONE_SIM
#elif PLATFORM_IPHONE
#elif PLATFORM_MAC
#elif PLATFORM_LINUX
#endif 
  }
