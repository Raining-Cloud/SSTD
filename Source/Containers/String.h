#pragma once

#include "General/Allocator.h"
#include "General/Meta.h"
#include "General/Memory.h"

namespace SSTD
{
  template <typename CharType, CharType NullTerminator, typename SizeType = size_t, template<typename> typename A = Allocator>
  class TString
  {
  public:
    using AllocType = A<CharType>;

    explicit TString() : m_Allocator{} {}

    TString(SizeType capacity) : m_Allocator()
    {
      if (capacity > SSOSize)
      {
        m_Data.long_string.capacity = capacity;
        m_Data.long_string.msb_capacity = Bit::MSB(capacity);
        m_Data.long_string.size = 0;
        m_Data.long_string.msb_size = ~0;
        m_Data.long_string.buffer = m_Allocator.Allocate(capacity);
      }
    }

    template <SizeType N>
    constexpr TString(const CharType(&str)[N]) : TString(str, N - 1) {}

    constexpr TString(const CharType* str, SizeType size)
    {
      if (str)
      {
        if (size > SSOSize)
        {
          size = Grow(size);
          m_Data.long_string.size = m_Data.long_string.capacity = size;
          m_Data.long_string.msb_capacity = Bit::MSB(m_Data.long_string.capacity);
          m_Data.long_string.msb_size = ~m_Data.long_string.msb_capacity;
          m_Data.long_string.buffer = m_Allocator.Allocate(size);
          TMemCpy<CharType>(m_Data.long_string.buffer, str, size);
        }
        else
        {
          m_Data.short_string.size = static_cast<CharType>((SSOSize - size) << 2);
          TMemCpy<CharType>(m_Data.short_string.buffer, str, size);
        }
      }
    }

    TString(const TString& other)
    {
      if (other.IsShort())
      {
        m_Data.short_string.size = other.m_Data.short_string.size;
        TMemCpy<CharType>(m_Data.short_string.buffer, other.m_Data.short_string.buffer, other.GetShortSize());
      }
      else
      {
        m_Data.long_string.size = other.m_Data.long_string.size;
        m_Data.long_string.msb_size = other.m_Data.long_string.msb_size;

        m_Data.long_string.capacity = other.m_Data.long_string.capacity;
        m_Data.long_string.msb_capacity = other.m_Data.long_string.msb_capacity;

        m_Data.long_string.buffer = m_Allocator.Allocate(other.GetLongCapacity());
        TMemCpy<CharType>(m_Data.long_string.buffer, other.m_Data.long_string.buffer, m_Data.long_string.size);
      }
    }

    TString(TString&& other) noexcept : m_Allocator
    {
      Move(other.m_Allocator)
    }
    {
      TMemCpy<uint8>(m_Data.raw_string, other.m_Data.raw_string, sizeof(LongString));

      other.m_Data.long_string.buffer = nullptr;
      other.m_Data.long_string.size = 0;
      other.m_Data.long_string.msb_size = 0;
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
      return false;
    }

    CharType& operator[](SizeType index)
    {
      return IsShort() ? m_Data.short_string.buffer[index] : m_Data.long_string.buffer[index];
    }
    const CharType& operator[](SizeType index) const
    {
      return IsShort() ? m_Data.short_string.buffer[index] : m_Data.long_string.buffer[index];
    }

    TString& operator=(const TString& other)
    {
      if (!IsShort())
        m_Allocator.Deallocate(m_Data.long_string.buffer);

      if (other.IsShort())
      {
        TMemCpy<CharType>(m_Data.short_string.buffer, other.m_Data.short_string.buffer, other.GetShortSize());
        m_Data.short_string.size = other.m_Data.short_string.size;
      }
      else
      {
        m_Data.long_string.capacity = other.m_Data.long_string.capacity;
        m_Data.long_string.msb_capacity = other.m_Data.long_string.msb_capacity;
        m_Data.long_string.size = other.m_Data.long_string.size;
        m_Data.long_string.msb_size = other.m_Data.long_string.msb_size;
        m_Data.long_string.buffer = m_Allocator.Allocate(m_Data.long_string.capacity);
        TMemCpy<CharType>(m_Data.long_string.buffer, other.m_Data.long_string.buffer, other.GetLongSize());
      }
      return *this;
    }

    TString& operator=(TString&& other) noexcept
    {
      if (!IsShort())
        m_Allocator.Deallocate(m_Data.long_string.buffer);

      m_Allocator = Move(other.m_Allocator);
      TMemCpy<uint8>(m_Data.raw_string, other.m_Data.raw_string, sizeof(LongString));

      other.m_Data.long_string.buffer = nullptr;
      other.m_Data.long_string.size = 0;
      other.m_Data.long_string.msb_size = 0;

      return *this;
    }


    CharType& At(SizeType index) 
    {
      if (IsShort())
      {
        if (index > GetShortSize())
          throw;
        return m_Data.short_string.buffer[index];
      }
      else
      {
        if (index > GetLongSize())
          throw;
        return m_Data.long_string.buffer[index];
      }
    }
    const CharType& At(SizeType index) const
    {
      if (IsShort())
      {
        if (index > GetShortSize())
          throw;
        return m_Data.short_string.buffer[index];
      }
      else
      {
        if (index > GetLongSize())
          throw;
        return m_Data.long_string.buffer[index];
      }
    }

    CharType& Front() { return IsShort() ? m_Data.short_string.buffer[0] : m_Data.long_string.buffer[0]; }
    const CharType& Front() const { return IsShort() ? m_Data.short_string.buffer[0] : m_Data.long_string.buffer[0]; }

    CharType& Back() { return IsShort() ? m_Data.short_string.buffer[GetShortSize()] : m_Data.long_string.buffer[GetLongSize()]; }
    const CharType& Back() const { return IsShort() ? m_Data.short_string.buffer[GetShortSize()] : m_Data.long_string.buffer[GetLongSize()]; }

    void Resize(SizeType size)
    {
      SizeType start_size = 0;
      if (IsShort())
      {
        start_size = GetShortSize();
        if (size > SSOSize)
        {
          auto temp = m_Allocator.Allocate(size);
          TMemCpy<CharType>(temp, m_Data.short_string.buffer, GetShortSize());
          m_Data.long_string.buffer = temp;
          m_Data.long_string.capacity = size;
          m_Data.long_string.size = size;
          m_Data.long_string.msb_capacity = Bit::MSB(size);
          m_Data.long_string.msb_size = ~m_Data.long_string.msb_capacity;
        }
        else
        {
          m_Data.short_string.size = (SSOSize - size) << 2;
          m_Data.short_string.buffer[size] = NullTerminator;
        }
      }
      else
      {
        start_size = GetLongSize();
        if (size > GetLongCapacity())
        {
          auto temp = m_Allocator.Allocate(size);
          TMemCpy<CharType>(temp, m_Data.long_string.buffer, GetShortSize());
          m_Allocator.Deallocate(m_Data.long_string.buffer);
          m_Data.long_string.buffer = temp;

          m_Data.long_string.capacity = size;
          m_Data.long_string.msb_capacity = Bit::MSB(size);
          m_Data.long_string.size = size;
          m_Data.long_string.msb_size = ~m_Data.long_string.msb_capacity;
        }
        else
        {
          m_Data.long_string.size = size;
          m_Data.long_string.msb_size = ~Bit::MSB(size);
          m_Data.long_string.buffer[size] = NullTerminator;
        }
      }
    }

    void Reserve(SizeType capacity)
    {
      if (IsShort())
      {
        SizeType c = Grow(SSOSize + capacity);
        SizeType size = GetShortSize();

        if (c < SSOSize) 
          return;
        auto temp = m_Allocator.Allocate(c);
        TMemCpy<CharType>(temp, m_Data.short_string.buffer, size);
        
        
        m_Data.long_string.buffer = temp;
        m_Data.long_string.capacity = c;
        m_Data.long_string.msb_capacity = c;
        m_Data.long_string.size = size;
        m_Data.long_string.msb_size = ~Bit::MSB(size);
      }
      else
      {
        SizeType c = Grow(GetLongSize() + capacity);
        SizeType size = GetLongSize();
        if (c < SSOSize)
          return;

        auto temp = m_Allocator.Allocate(c);

        TMemCpy<CharType>(temp, m_Data.long_string.buffer, GetLongSize());
        m_Allocator.Deallocate(m_Data.long_string.buffer);
        m_Data.long_string.size = size;

        m_Data.long_string.buffer = temp;
        m_Data.long_string.capacity = c;
        m_Data.long_string.msb_capacity = c;
      }
    }

    void Minimize()
    {
      Resize(Size());
    }

    void Clear()
    {
      if (!IsShort())
        m_Allocator.Deallocate(m_Data.long_string.buffer);

      for (auto i = 0; i < SSOSize; i++)
        m_Data.raw_string[i] = 0;
    }

    void Append(const TString& other)
    {
      SizeType s = other.Size();
      Reserve(s);
      if (IsShort())
      {
        TMemCpy<CharType>(m_Data.short_string.buffer + GetShortSize(), other.CStr(), s);
        m_Data.long_string.size = GetShortSize() + s;
      }
      else
      {
        TMemCpy<CharType>(m_Data.long_string.buffer + GetLongSize(), other.CStr(), s);
        m_Data.long_string.size += s;
      }
    }

    const CharType* CStr() const
    {
      return IsShort() ? m_Data.short_string.buffer : m_Data.long_string.buffer;
    }

    SizeType Size() const
    {
      return IsShort() ? SSOSize - (m_Data.short_string.size >> 2) : Bit::ApplyMSB(static_cast<SizeType> (m_Data.long_string.size), !m_Data.long_string.size);
    }

    SizeType Capacity() const
    {
      return IsShort() ? SSOSize : Bit::ApplyMSB(static_cast<SizeType> (m_Data.long_string.capacity), m_Data.long_string.msb_capacity);
    }

    bool IsEmpty() const
    {
      return Size() == 0;
    }

  private:

    SizeType GetShortSize() const
    {
      return static_cast<SizeType>(SSOSize) - (m_Data.short_string.size >> 2);
    }

    SizeType GetLongSize() const
    {
      return Bit::ApplyMSB(static_cast<SizeType> (m_Data.long_string.size), !m_Data.long_string.size);
    }

     SizeType GetLongCapacity() const
    {
      return Bit::ApplyMSB(static_cast<SizeType> (m_Data.long_string.capacity), m_Data.long_string.msb_capacity);
    }

     bool IsShort() const
    {
      return !m_Data.long_string.msb_capacity & !m_Data.long_string.msb_size;
    }

    constexpr SizeType Grow(SizeType s) const
    {
      return static_cast<SizeType>(s * 1.6);
    }

    struct LongString
    {
      CharType* buffer;
      SizeType capacity : sizeof(SizeType) * 8 - 1;
      SizeType msb_capacity : 1;
      SizeType size : sizeof(SizeType) * 8 - 1;
      SizeType msb_size : 1;
    };

    static const size_t SSOSize = (sizeof(LongString) / sizeof(CharType));

    struct ShortString
    {
      CharType buffer[SSOSize - 1];
      CharType size;
    };

    union Data
    {
      LongString long_string;
      ShortString short_string;
      uint8 raw_string[sizeof(LongString)] = { 0 };
    }
    m_Data;

    [[msvc::no_unique_address]] AllocType m_Allocator;	//TODO: this is MSVC specific, we may need to work with different compilers at some point
  };

  using String   = TString<char, '\0'>;
  using WString = TString<wchar_t, L'\0'>;
}