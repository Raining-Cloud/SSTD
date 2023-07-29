#pragma once

#include "Platform/IncludePlatform.h"

#include "General/Numeric.h"
#include "Containers/String.h"
#include "Containers/Color.h"
#include "Containers/Vector.h"

#include "WindowEvent.h"

namespace SSTD
{

  enum class CursorMode
  {
    None,
    Visible,
    Hidden
  };

  enum class WindowStyle
  {
    Windowed,
    Borderless,
  };

  struct WindowDesc
  {
    uint32 x = 0;
    uint32 y = 0;

    uint32 width = 1920;
    uint32 height = 1080;

    uint32 min_width = 1920/2;
    uint32 min_height = 1080/2;

    uint32 max_width = NumericLimit<uint32>::max;
    uint32 max_height = NumericLimit<uint32>::max;

    bool moveable = true;
    bool closeable = true;
    bool minimizeable = true;
    bool maximizeable = true;

    Color background_color = Colors::DarkGrey;
    Color titlebar_color = Colors::DarkGrey;
    WindowStyle window_style = WindowStyle::Windowed;
    bool shadow = true;

    String title = "Window";
    String name = "WindowApplication";
    String icon_path = "";
  };

  class Window
  {
  public:
    using WindowEventQueue = Vector<WindowEvent>;

    Window() = delete;

    void Procces();
    const WindowEventQueue& BeginEvents() const;
    void EndEvents();

    void Realign(uint32 width, uint32 height, uint32 x, uint32 y);

    void SetPosition(uint32 x, uint32 height);
    uint32 GetPositionX() const { return m_Desc.x; }
    uint32 GetPositionY() const { return m_Desc.y; }

    void SetSize(uint32 width, uint32 height);
    uint32 GetWidth() const { return m_Desc.width; }
    uint32 GetHeight() const { return m_Desc.height; }

    void SetTitle(const String& title);
    String GetTitle() const { return m_Desc.title; }

    void SetBackgroundColor(const Color& col) { m_Desc.background_color = col; }
    Color GetBackgroundColor() { return m_Desc.background_color; }

    void SetTitleBarColor(const Color& col);
    Color GetTitleBarColor() { return m_Desc.titlebar_color; }

    String GetName() const { return m_Desc.name; }

    void* GetNative() const;

    void Close();

    void Minimize();
    bool IsMinimized() const;

    void Maximize();
    bool IsMaximized() const;

    void Hide();
    bool IsHidden();

    void Show();
    bool IsVisible() const;

    void Focus();
    bool IsFocused() const;

    void Fullscreen(bool state);
    bool IsFullscreen() const;

    void Center();

    static Window* Create(const WindowDesc& desc) { return new Window(desc); }
    static void Delete(Window* window) { delete window; }

  private:  
    Window(const WindowDesc& desc);
    ~Window();

#ifdef PLATFORM_WIN64

    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    DWORD ToNative(WindowStyle style) const;

    HWND m_HWND = nullptr;
#elif PLATFORM_WIN32
#elif PLATFORM_IPHONE_SIM
#elif PLATFORM_IPHONE
#elif PLATFORM_MAC
#elif PLATFORM_LINUX
#endif
    WindowDesc m_Desc;
    WindowEventQueue m_EventQueue;
  };
}