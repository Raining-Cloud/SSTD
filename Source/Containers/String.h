#pragma once

#include "General/Allocator.h"
#include "General/Meta.h"
#include "General/Memory.h"

#include "General/Exception.h"
#include "General/Utility.h"

#include "Containers/Pair.h"

#include <memory>

namespace SSTD
{
  template <typename CharType, CharType NullTerminator, typename SizeType = size_t, template<typename> typename A = Allocator, typename G = GenericGrow>
  class TString
  {
  public:
    using AllocType = A<CharType>;

    TString()
      : m_Allocator{}
    {
      SetShortSize(0);
    }

    explicit TString(const AllocType& alloc)
      : m_Allocator{ alloc }
    {
      SetShortSize(0);
    }

    TString(const CharType* str, SizeType size, const AllocType& alloc = AllocType())
      :m_Allocator{ alloc }
    {
      if (size > m_Data.s.capacity)
      {
        m_Data.l.buffer = TMemCpy<CharType>(m_Allocator.Allocate(size + 1), str, size);
        m_Data.l.capacity = size;
        SetLongSize(size);
      }
      else
      {
        TMemCpy<CharType>(m_Data.s.buffer, str, size);
        SetShortSize(size);
      }

    }

    template <SizeType N>
    constexpr TString(const CharType(&str)[N], const AllocType& alloc = AllocType())
      : TString(str, N - 1, alloc)
    {}

    TString(const TString& other)
      : m_Allocator{ other.m_Allocator }
    {
      if (other.GetState() == State::Short)
      {
        m_Data = other.m_Data;
      }
      else
      {
        const SizeType size = other.GetLongSize();
        m_Data.l.buffer = TMemCpy<CharType>(m_Allocator.Allocate(size + 1), other.m_Data.l.buffer, size);
        m_Data.l.capacity = size;
        SetLongSize(size);
      }
    }

    TString(TString&& other) noexcept
      : m_Data(other.m_Data)
    {
      m_Data = other.m_Data;
      other.SetShortSize(0);
    }

    ~TString()
    {
      Clear();
    }

    bool operator==(const TString& other) const
    {
      if (Size() != other.Size())
        return false;

      const CharType* a = CStr();
      const CharType* b = other.CStr();

      while ((*a != NullTerminator) && (*a == *b))
      {
        ++a;
        ++b;
      }

      return (*a == *b);
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
      Clear();

      return *this;
    }

    TString& operator=(TString&& other) noexcept
    {
      Clear();

      m_Data = other.m_Data;
      other.SetShortSize(0);
      return *this;
    }

    TString& operator+=(const TString& other)
    {
      Append(other);
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

    CharType& Front() { return CStr()[0]; }
    const CharType& Front() const { return CStr()[0]; }

    CharType& Back() { return CStr()[Size()]; }
    const CharType& Back() const { return CStr()[Size()]; }

    void Resize(SizeType size, CharType fill = static_cast<CharType>('0'))
    {
      Reserve(size);
      const State state = GetState();
      const SizeType old_size = Size();


      for (SizeType i = old_size; i < size; i++)
        CStr()[i] = fill;

      if (state == State::Long)
        SetLongSize(size);
      else
        SetShortSize(size);

    }

    void Reserve(SizeType capacity)
    {
      const SizeType size = Size();
      const CharType* cstr = CStr();
      const SizeType cap = Capacity();

      if (capacity <= cap)
        return;

      const SizeType new_cap = (capacity + size) * 1.666;
      CharType* tmp = TMemCpy<CharType>(m_Allocator.Allocate(new_cap + 1), cstr, size);

      if (cap > m_Data.s.capacity)
        m_Allocator.Deallocate(m_Data.l.buffer);

      m_Data.l.buffer = tmp;
      m_Data.l.capacity = new_cap;
      SetLongSize(size);
    }

    void Minimize()
    {
      const SizeType size = Size();
      const CharType* cstr = CStr();
      const SizeType capacity = Capacity();

      if ((capacity < m_Data.s.capacity) || (capacity == size)) //check if we actually need to allocate new heap memory
        return;

      CharType* tmp = TMemCpy<CharType>(m_Allocator.Allocate(size + 1), cstr, size);
      m_Allocator.Deallocate(m_Data.l.buffer);

      m_Data.l.buffer = tmp;
      m_Data.l.capacity = size;
      SetLongSize(size);
    }

    void Append(const TString& other)
    {
      Append(other.CStr(), other.Size());
    }

    void Append(const CharType* str, SizeType size)
    {
      const SizeType old_size = Size();
      const SizeType new_size = size + old_size;

      if (new_size <= m_Data.s.capacity)
      {
        TMemCpy<CharType>(m_Data.s.buffer + old_size, str, size);
        SetShortSize(new_size);
        return;
      }

      CharType* tmp = TMemCpy<CharType>(m_Allocator.Allocate(new_size + 1), m_Data.l.buffer, old_size);
      TMemCpy<CharType>(tmp + old_size, str, size);

      if (old_size <= m_Data.s.capacity)
        m_Allocator.Deallocate(m_Data.l.buffer);

      m_Data.l.buffer = tmp;
      m_Data.l.capacity = new_size;
      SetLongSize(new_size);
    }

    void Clear()
    {
      const State state = GetState();
      if (state == State::Long)
        m_Allocator.Deallocate(m_Data.l.buffer);
      SetShortSize(0);
    }

    CharType const* CStr() const
    {
      const State state = GetState();
      switch (state)
      {
      case State::Long:
        return m_Data.l.buffer;
      case State::Short:
        return m_Data.s.buffer;
      }
    }

    SizeType Size() const
    {
      const State state = GetState();
      switch (state)
      {
      case State::Long:
        return GetLongSize();
      case State::Short:
        return GetShortSize();
      }
    }

    SizeType Capacity() const
    {
      const State state = GetState();
      switch (state)
      {
      case State::Long:
        return m_Data.l.capacity;
      case State::Short:
        return m_Data.s.capacity;
      }
    }

    bool IsEmpty() const
    {
      return Size() == 0;
    }

    //private:

    enum class State
    {
      Short = 0x00,
      Long = 0x01,
      Mask = 0b11,
    };

    State GetState() const
    {
      return static_cast<State>(m_Data.s.state & static_cast<SizeType>(State::Mask));
    }

    //-----------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------

    SizeType GetLongSize() const
    {
      return m_Data.l.size;
    }

    void SetLongSize(SizeType size)
    {
      m_Data.s.state = static_cast<CharType>(State::Long);
      m_Data.l.size = size;
      m_Data.l.buffer[size] = NullTerminator;
    }

    //-----------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------

    SizeType GetShortSize() const
    {
      return m_Data.s.capacity - (m_Data.s.state >> 2);
    }

    void SetShortSize(SizeType size)
    {
      m_Data.s.state = ((m_Data.s.capacity - size) << 2) | static_cast<CharType>(State::Short);
      m_Data.raw[size + 1] = NullTerminator;
    }

    union TStringData
    {
      struct LongStr//this struct holds all the data for the representation of long strings (allocated on the heap), for providing the same acsess as in the short representation the size also sets the state 
      {
        CharType* buffer;
        SizeType capacity;
        SizeType size;
        uint8 padding; //this is ONLY for the calculation of the final size !
      } l;

      struct ShortStr//this struct holds all the data for the small string which fit on the stack, we cannot easily set the size like in the long representation, so we need the size() function
      {
        static constexpr size_t capacity = (sizeof(LongStr) - sizeof(CharType)) / sizeof(CharType);
        CharType buffer[capacity];
        CharType state;
      } s;

      CharType raw[sizeof(LongStr) / sizeof(CharType)];

    } m_Data;

    [[no_unique_address]] AllocType m_Allocator;

  };

  using String = TString<char, '\0'>;
  using WString = TString<wchar_t, L'\0'>;
}
