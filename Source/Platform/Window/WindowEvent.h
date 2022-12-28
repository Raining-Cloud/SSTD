#pragma once

#include "General/Numeric.h"
#include "Platform/IO/InputDeclarations.h"
#include "Math/Vec.h"
#include "Containers/Queue.h"

namespace SSTD
{
  enum class EventType
  {
    None,
    Create,
    Close,
    Resize,
    Move,
    Focus,
    Paint,
    DPI,
    CharData,
    KeyboardInput,
    MouseInput,
    MouseMove,
    MouseWheel,
    Touch,
    Gamepad,
    MAX
  };


  struct None
  {
    static constexpr EventType type = EventType::None;
  };

  struct FocusData
  {
    bool is_focused;
    static constexpr EventType type = EventType::Focus;

  };

  struct ResizeData
  {
    uint32 width;
    uint32 height;
    static constexpr EventType type = EventType::Resize;
  };

  struct DPIData
  {
    float scale;
    static constexpr EventType type = EventType::DPI;
  };

  struct CharData
  {
    char c;
    static constexpr EventType type = EventType::CharData;
  };

  struct KeyboardInputData
  {
    KeyCode key;
    ButtonState state;
    static constexpr EventType type = EventType::KeyboardInput;
  };

  struct MouseInputData
  {
    MouseCode key;
    ButtonState state;
    int32 x;
    int32 y;
    static constexpr EventType type = EventType::MouseInput;
  };

  struct MouseMoveData
  {
    int32 x;
    int32 y;
    static constexpr EventType type = EventType::MouseMove;
  };

  struct MouseWheelData
  {
    short delta;
    static constexpr EventType type = EventType::MouseWheel;
  };

  struct TouchData
  {
    uint64 touch_id;
    int32 local_x;
    int32 local_y;
    int32 global_x;
    int32 global_y;
    bool changed;
    static constexpr EventType type = EventType::Touch;
  };

  struct GamepadData
  {
    static constexpr EventType type = EventType::Gamepad;
  };

  class WindowEvent
  {
  public:
    WindowEvent() : m_Type(EventType::None) {}
    WindowEvent(const WindowEvent& other) : m_Type(other.m_Type) { m_Data = other.m_Data; }
    WindowEvent(EventType e) : m_Type(e) {}

    WindowEvent(FocusData d) : m_Type(d.type) { m_Data.focus_data = d; }
    WindowEvent(ResizeData d) : m_Type(d.type) { m_Data.resize_data = d; }
    WindowEvent(DPIData d) : m_Type(d.type) { m_Data.dpi_data = d; }
    WindowEvent(CharData d) : m_Type(d.type) { m_Data.char_data = d; }
    WindowEvent(KeyboardInputData d) : m_Type(d.type) { m_Data.keyboard_data = d; }
    WindowEvent(MouseInputData d) : m_Type(d.type) { m_Data.mouse_button_data = d; }
    WindowEvent(MouseMoveData d) : m_Type(d.type) { m_Data.mouse_move_data = d; }
    WindowEvent(MouseWheelData d) : m_Type(d.type) { m_Data.mouse_wheel_data = d; }
    WindowEvent(TouchData d) : m_Type(d.type) { m_Data.touch_data = d; }
    WindowEvent(GamepadData d) : m_Type(d.type) { m_Data.gamepad_data = d; }

    EventType Type() const { return m_Type; }

    union EventData
    {
      None none{};
      FocusData focus_data;
      ResizeData resize_data;
      DPIData dpi_data;
      CharData char_data;
      KeyboardInputData keyboard_data;
      MouseInputData mouse_button_data;
      MouseMoveData mouse_move_data;
      MouseWheelData mouse_wheel_data;
      TouchData touch_data;
      GamepadData gamepad_data;
    } m_Data;
  private:
    EventType m_Type;
  };
}