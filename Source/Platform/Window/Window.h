#pragma once

#include "Platform/IncludePlatform.h"

#include "General/Numeric.h"
#include "Containers/String.h"
#include "Containers/Color.h"
#include "Containers/Vector.h"

#include "WindowEvent.h"

//The key is to isolate non-portable parts of the code from the main code base.

namespace SSTD
{

  enum class CursorMode : uint8
  {
    None,
    Visible,
    Hidden
  };

  enum class WindowStyle
  {
    Normal,
    Borderless,
    PopUp,
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

    bool centered = true;
    bool moveable = true;
    bool closeable = true;
    bool minimizeable = true;
    bool maximizeable = true;


    Color background_color = Colors::dark_grey;
    WindowStyle window_style = WindowStyle::Normal;
    bool shadow = true;

    bool visible = true;
    bool fullscreen = false;

    String title = "Window";
    String name = "WindowApplication";
    String icon = "";
  };

  class Window
  {
  public:
    //Init Values
    explicit Window();
    Window(const WindowDesc& desc);
    ~Window();

    void Procces();
    auto BeginEvents() const;
    void EndEvents();

    void Realign(uint32 width, uint32 height, uint32 x, uint32 y);

    void UpdateDesc(const WindowDesc& desc) { throw; }

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

    void* GetNative();

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
  private:
    using WindowEventQueue = Vector<WindowEvent>;
#ifdef PLATFORM_WIN64

    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    DWORD ToNative(WindowStyle style) const;

    HWND m_WindowHandle = nullptr;
    WINDOWPLACEMENT m_WindowPlacement = { };
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