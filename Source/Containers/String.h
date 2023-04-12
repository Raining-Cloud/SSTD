#pragma once

#include "General/Allocator.h"
#include "General/Meta.h"
#include "General/Memory.h"

#include "General/Exception.h"
#include "General/Utility.h"

#include "Containers/Pair.h"

namespace SSTD
{
  template <typename CharType, CharType NullTerminator, typename SizeType = size_t, template<typename> typename A = Allocator>
  class TString
  {
  public:
    using AllocType = A<CharType>;

    explicit TString(const AllocType& alloc = AllocType()) : m_Allocator{ alloc }
    {
      SetStateData(StringState::Short, 1);
    }

    TString(const CharType* str, SizeType size, const AllocType& alloc = AllocType())
      : m_Allocator{ alloc }
    {
      if (size > m_ShortStr.capacity)
      {
        SetStateData(StringState::Long);
        m_LongStr.buffer = TMemCpy<CharType>(m_Allocator.Allocate(size), str, size);
        m_LongStr.capacity = size;
        m_LongStr.size = size;
      }
      else
      {
        SetStateData(StringState::Short, size);
        TMemCpy<CharType>(m_ShortStr.buffer, str, size);
      }
    }

    template <SizeType N>
    constexpr TString(const CharType(&str)[N], const AllocType& alloc = AllocType()) :TString(str, N, alloc) {}

    TString(const TString& other)
      : m_Allocator{ other.m_Allocator }
    {
      const Pair<StringState, uint8> sdata = other.GetStateData();
      if (sdata.first == StringState::Short)
      {
        TMemCpy<uint8>(m_RawStr.buffer, other.m_RawStr.buffer, sizeof(m_RawStr.buffer));
        m_RawStr.state = other.m_RawStr.state;
      }
      else if (sdata.first == StringState::Long)
      {
        SetStateData(StringState::Long);
        m_LongStr.buffer = TMemCpy<CharType>(m_Allocator.Allocate(other.m_LongStr.size), other.m_LongStr.buffer, other.m_LongStr.size);
        m_LongStr.capacity = other.m_LongStr.size;
        m_LongStr.size = other.m_LongStr.size;
      }
    }

    TString(TString&& other) noexcept : m_Allocator{ Move(other.m_Allocator) }
    {

      TMemCpy<uint8>(m_RawStr.buffer, other.m_RawStr.buffer, sizeof(m_RawStr.buffer));
      m_RawStr.state = other.m_RawStr.state;
      SetStateData(StringState::Short, 1);
      other.m_ShortStr.buffer[0] = NullTerminator;
    }

    ~TString()
    {
      Clear();
    }

    bool operator==(const TString& other)
    {
      return false;
    }
    bool operator!=(const TString& other)
    {
      return !(*this == other);
    }

    CharType& operator[](SizeType index)
    {
      return CStr()[index];
    }
    const CharType& operator[](SizeType index) const
    {
      return CStr()[index];
    }

    TString& operator=(const TString& other)
    {
      return *this;
    }

    TString& operator=(TString&& other) noexcept
    {
      return *this;
    }

    CharType& At(SizeType index)
    {
      return operator[](index);
    }

    const CharType& At(SizeType index) const
    {
      return operator[](index);
    }

    CharType& Front() { }
    const CharType& Front() const { }

    CharType& Back() { }
    const CharType& Back() const { }

    void Resize(SizeType size, CharType value = '0')
    {

    }

    void Reserve(SizeType capacity)
    {
      if (capacity <= m_ShortStr.capacity)
        return;

      CharType* cstr{};
      SizeType size{};

      const Pair<StringState, uint8> sdata = GetStateData();
      if (sdata.first == StringState::Short)
      {
        if (capacity < sdata.second)
          return;
        cstr = m_ShortStr.buffer;
        size = sdata.second;
      }
      else if (sdata.first == StringState::Long)
      {
        if (capacity < m_LongStr.size)
          return;
        cstr = m_LongStr.buffer;
        size = m_LongStr.size;
      }
      SetStateData(StringState::Long);
      const SizeType new_cap = capacity + size;

      CharType* tmp = m_Allocator.Allocate(new_cap);
      TMemCpy<CharType>(tmp, cstr, size);
      m_LongStr.buffer = tmp;
      m_LongStr.size = size;
      m_LongStr.capacity = new_cap;
    }

    void Minimize()
    {
      const Pair<StringState, uint8> sdata = GetStateData();
      if (sdata.first == StringState::Long)
      {
        CharType tmp = m_Allocator.Allocate(m_LongStr.size);
        TMemCpy<CharType>(tmp, m_LongStr.buffer, m_LongStr.size);
        m_LongStr.capacity = m_LongStr.size;
      }
    }

    void Clear()
    {
      const Pair<StringState, uint8> sdata = GetStateData();
      if (sdata.first == StringState::Long)
        m_Allocator.Deallocate(m_LongStr.buffer);

      SetStateData(StringState::Short, 1);
      m_ShortStr.buffer[0] = NullTerminator;
    }

    const CharType* CStr() const
    {
      const Pair<StringState, uint8> sdata = GetStateData();
      if (sdata.first == StringState::Short)
        return m_ShortStr.buffer;
      else if (sdata.first == StringState::Long)
        return m_LongStr.buffer;
    }

    SizeType Size() const
    {
      const Pair<StringState, uint8> sdata = GetStateData();
      if (sdata.first == StringState::Short)
        return sdata.second - 1;
      else if (sdata.first == StringState::Long)
        return m_LongStr.size - 1;
    }

    SizeType Capacity() const
    {
      const Pair<StringState, uint8> sdata = GetStateData();
      if (sdata.first == StringState::Short)
        return m_ShortStr.capacity;
      else if (sdata.first == StringState::Long)
        return m_LongStr.capacity - 1;
    }

    bool IsEmpty() const
    {
      return true;
    }

  private:

    enum StringState : uint8
    {
      Short = 0x00,
      Long = 0x01,
    };
    static constexpr size_t StateMask = 0b11; // least two bits set to one

    Pair<StringState, uint8> GetStateData() const
    {
      return { static_cast<StringState>(m_RawStr.state & StateMask), static_cast<uint8>(m_RawStr.state >> 2) };
    }

    void SetStateData(const StringState state, uint8 data = 0)
    {
      m_RawStr.state = (data << 2) | state;
    }

    union
    {
      struct LongStr
      {
        CharType* buffer;
        SizeType size;
        SizeType capacity;
      private:
        uint8 state;
      } m_LongStr;

      struct RawStr
      {
        uint8 buffer[sizeof(LongStr) - 1]{0};
        uint8 state{ 0 };
      } m_RawStr{};

      struct ShortStr
      {
        static constexpr size_t capacity = (sizeof(LongStr) - sizeof(StringState)) / sizeof(CharType);
        CharType buffer[capacity];
      } m_ShortStr;
    };

    [[msvc::no_unique_address]] AllocType m_Allocator;	//TODO: this is MSVC specific, we may need to work with different compilers at some point
  };

  using String = TString<char, '\0'>;
  using WString = TString<wchar_t, L'\0'>;
}