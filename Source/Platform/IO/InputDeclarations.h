#pragma once

#include "Platform/IncludePlatform.h"
#include "General/Numeric.h"


namespace SSTD
{
  enum class ButtonState : uint8
  {
    Pressed = true,
    Released = false,
  };

  //all keys on your keyboard
  enum class KeyCode
  {
    None,
    Backspace,
    Space,
    Enter,
    Escape,
    Tab,
    CapsLock,
    LeftShift,
    RightShift,
    LeftControl,
    RightControl,
    LeftOSKey,
    RightOSKey,
    Alt,
    AltGr,
    Print,
    Scroll,
    Pause,
    Insert,
    Delete,
    Pos1,
    End,
    PageUp,
    PageDown,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    NumLock,
    Numpad0,
    Numpad1,
    Numpad2,
    Numpad3,
    Numpad4,
    Numpad5,
    Numpad6,
    Numpad7,
    Numpad8,
    Numpad9,
    Divide,
    Multiply,
    Subtract,
    Add,
    ArrowLeft,
    ArrowRight,
    ArrowUp,
    ArrowDown,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Num0,
    Q, W, E, R, T, Z, U, I, O, P,
    A, S, D, F, G, H, J, K, L,
    Y, X, C, V, B, N, M,
    Exclaim,
    Question,
    Quote,
    DoubleQuote,
    BackQuote,
    Hashtag,
    Dollar,
    Percent,
    Ampersand, //&
    LeftParen,
    RightParen,
    LeftBracket,
    RightBracket,
    LeftCurlyBracket,
    RightCurlyBracket,
    Asterisk,
    Tilde,
    Plus,
    Minus,
    Dot,
    Slash,
    BackSlash,
    Colon,
    SemiColon,

    Less,
    Equal,
    Bigger,
    At,
    Caret,
    Underscore,

    MAX
  };

  enum class MouseCode
  {
    None,
    LeftMouse,
    RightMouse,
    MiddleMouse,
    Mouse0,
    Mouse1,
    Mouse3,
    Mouse4,
    MAX
  };

  struct ShortcutKey
  {
    KeyCode key;
    ShortcutKey(KeyCode k) : key(k) {}
    bool Control() const { return key == KeyCode::LeftControl || key == KeyCode::RightControl; }
  };

#ifdef PLATFORM_WIN64
  static constexpr KeyCode ToKeyCode(uint8 vkc)
  {
    KeyCode k;
    switch (vkc)
    {
    case 0x08:
      k = KeyCode::Backspace;
      break;
    case 0x20:
      k = KeyCode::Space;
      break;
    case 0x0D:
      k = KeyCode::Enter;
      break;
    case 0x1B:
      k = KeyCode::Escape;
      break;
    case 0x09:
      k = KeyCode::Tab;
      break;
    case 0x14:
      k = KeyCode::CapsLock;
      break;
    case 0xA0:
      k = KeyCode::LeftShift;
      break;
    case 0xA1:
      k = KeyCode::RightShift;
      break;
    case 0xA2:
      k = KeyCode::LeftControl;
      break;
    case 0xA3:
      k = KeyCode::RightControl;
      break;
    case 0x5B:
      k = KeyCode::LeftOSKey;
      break;
    case 0x5c:
      k = KeyCode::RightOSKey;
      break;
    case 0xA4:
      k = KeyCode::Alt;
      break;
    case 0xA5:
      k = KeyCode::AltGr; //do we need two alt keys ?!
      break;
    case 0x2A:
      k = KeyCode::Print;
      break;
    case 0x91:
      k = KeyCode::Scroll;
      break;
    case 0x13:
      k = KeyCode::Pause;
      break;
    case 0x2D:
      k = KeyCode::Insert;
      break;
    case 0x2E:
      k = KeyCode::Delete;
      break;
    case 0x24:
      k = KeyCode::Pos1;
      break;
    case 0x23:
      k = KeyCode::End;
      break;
    case 0x21:
      k = KeyCode::PageUp;
      break;
    case 0x22:
      k = KeyCode::PageDown;
      break;
    case 0x70:
      k = KeyCode::F1;
      break;
    case 0x71:
      k = KeyCode::F2;
      break;
    case 0x72:
      k = KeyCode::F3;
      break;
    case 0x73:
      k = KeyCode::F4;
      break;
    case 0x74:
      k = KeyCode::F5;
      break;
    case 0x75:
      k = KeyCode::F6;
      break;
    case 0x76:
      k = KeyCode::F7;
      break;
    case 0x77:
      k = KeyCode::F8;
      break;
    case 0x78:
      k = KeyCode::F9;
      break;
    case 0x79:
      k = KeyCode::F10;
      break;
    case 0x7A:
      k = KeyCode::F11;
      break;
    case 0x7B:
      k = KeyCode::F12;
      break;
    case 0x7C:
      k = KeyCode::F13;
      break;
    case 0x7D:
      k = KeyCode::F14;
      break;
    case 0x7E:
      k = KeyCode::F15;
      break;
    case 0x7F:
      k = KeyCode::F16;
      break;
    case 0x80:
      k = KeyCode::F17;
      break;
    case 0x81:
      k = KeyCode::F18;
    case 0x82:
      k = KeyCode::F19;
      break;
    case 0x83:
      k = KeyCode::F20;
      break;
    case 0x90:
      k = KeyCode::NumLock;
      break;
    case 0x60:
      k = KeyCode::Numpad0;
      break;
    case 0x61:
      k = KeyCode::Numpad1;
      break;
    case 0x62:
      k = KeyCode::Numpad2;
      break;
    case 0x63:
      k = KeyCode::Numpad3;
      break;
    case 0x64:
      k = KeyCode::Numpad4;
      break;
    case 0x65:
      k = KeyCode::Numpad5;
      break;
    case 0x66:
      k = KeyCode::Numpad6;
      break;
    case 0x67:
      k = KeyCode::Numpad7;
      break;
    case 0x68:
      k = KeyCode::Numpad8;
      break;
    case 0x69:
      k = KeyCode::Numpad9;
      break;
    case 0x6F:
      k = KeyCode::Divide;
      break;
    case 0x6A:
      k = KeyCode::Multiply;
      break;
    case 0x6D:
      k = KeyCode::Subtract;
      break;
    case 0x6B:
      k = KeyCode::Add;
      break;
    case 0x25:
      k = KeyCode::ArrowLeft;
      break;
    case 0x27:
      k = KeyCode::ArrowRight;
      break;
    case 0x26:
      k = KeyCode::ArrowUp;
      break;
    case 0x28:
      k = KeyCode::ArrowDown;
      break;
    case 0x30:
      k = KeyCode::Num0;
      break;
    case 0x31:
      k = KeyCode::Num1;
      break;
    case 0x32:
      k = KeyCode::Num2;
      break;
    case 0x33:
      k = KeyCode::Num3;
      break;
    case 0x34:
      k = KeyCode::Num4;
      break;
    case 0x35:
      k = KeyCode::Num5;
      break;
    case 0x36:
      k = KeyCode::Num6;
      break;
    case 0x37:
      k = KeyCode::Num7;
      break;
    case 0x38:
      k = KeyCode::Num8;
      break;
    case 0x39:
      k = KeyCode::Num9;
      break;
    case 0x41:
      k = KeyCode::A;
      break;
    case 0x42:
      k = KeyCode::B;
      break;
    case 0x43:
      k = KeyCode::C;
      break;
    case 0x44:
      k = KeyCode::D;
      break;
    case 0x45:
      k = KeyCode::E;
      break;
    case 0x46:
      k = KeyCode::F;
      break;
    case 0x47:
      k = KeyCode::G;
      break;
    case 0x48:
      k = KeyCode::H;
      break;
    case 0x49:
      k = KeyCode::I;
      break;
    case 0x4A:
      k = KeyCode::J;
      break;
    case 0x4B:
      k = KeyCode::K;
      break;
    case 0x4C:
      k = KeyCode::L;
      break;
    case 0x4D:
      k = KeyCode::M;
      break;
    case 0x4E:
      k = KeyCode::N;
      break;
    case 0x4F:
      k = KeyCode::O;
      break;
    case 0x50:
      k = KeyCode::P;
      break;
    case 0x51:
      k = KeyCode::Q;
      break;
    case 0x52:
      k = KeyCode::R;
      break;
    case 0x53:
      k = KeyCode::S;
      break;
    case 0x54:
      k = KeyCode::T;
      break;
    case 0x55:
      k = KeyCode::U;
      break;
    case 0x56:
      k = KeyCode::V;
      break;
    case 0x57:
      k = KeyCode::W;
      break;
    case 0x58:
      k = KeyCode::X;
      break;
    case 0x59:
      k = KeyCode::Y;
      break;
    case 0x5A:
      k = KeyCode::Z;
      break;
    default:
      k = KeyCode::None;
      break;
    }
    return k;
  }
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif 
}