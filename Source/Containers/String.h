#pragma once

#include "General/Allocator.h"
#include "General/Meta.h"
#include "General/Memory.h"

#include "General/Utility.h"
#include "General/Result.h"

namespace SSTD
{

  template <typename CharType, CharType NullTerminator, typename SizeType = size_t, template<typename> typename A = Allocator, typename G = GenericGrow>
  class TString
  {
  public:
    using AllocType = A<CharType>;

    explicit TString() : m_Allocator{} {}

    TString(const CharType* str, SizeType size, const AllocType& alloc = AllocType())
      :m_Allocator{ alloc }
    {
      if (size > m_Data.s.capacity)
      {
        const auto tmp = m_Allocator.Allocate(size + 1);
        TMemCpy<CharType>(tmp.ptr, str, size);
        m_Data.l.mem = tmp;
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
    {
      if (other.GetState() == State::Short)
      {
        m_Data = other.m_Data;
        return;
      }

      const SizeType size = other.GetLongSize();

      const auto tmp = m_Allocator.Allocate(size + 1);
      TMemCpy<CharType>(tmp.ptr, other.m_Data.l.mem.ptr, size);
      m_Data.l.mem = tmp;

      SetLongSize(size);
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

    Result<const CharType&> At(SizeType index) const
    {
      const auto size = Size();
      const auto cstr = CStr();
      if (index > size)
        return Result<CharType&>();

      return Result<const CharType&>(CStr()[index]);
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
      const SizeType cap = Capacity();

      if (capacity <= cap)
        return;

      const SizeType size = Size();
      const CharType* cstr = CStr();

      const SizeType new_cap = G::Grow((capacity));
      const auto tmp = m_Allocator.Allocate(new_cap + 1);
      TMemCpy<CharType>(tmp.ptr, cstr, size);

      if (cap > m_Data.s.capacity)
        m_Allocator.Deallocate(m_Data.l.mem);

      m_Data.l.mem = tmp;
      SetLongSize(size);
    }

    void Minimize()
    {
      const SizeType size = Size();
      const CharType* cstr = CStr();
      const SizeType capacity = Capacity();

      if ((capacity < m_Data.s.capacity) || (capacity == size)) //check if we actually need to allocate new heap memory
        return;

      const auto tmp = m_Allocator.Allocate(size + 1);
      TMemCpy<CharType>(tmp.ptr, cstr, size);

      m_Allocator.Deallocate(m_Data.l.mem);

      m_Data.l.mem = tmp;
      SetLongSize(size);
    }

    void Append(const TString& other)
    {
      Append(other.CStr(), other.Size());
    }

    void Append(const CharType* str, SizeType size)
    {
      const SizeType old_size = Size();
      const SizeType old_cap = Capacity();
      CharType* const data = Data();

      const SizeType new_size = old_size + size;

      if (new_size <= old_cap)
      {
        TMemCpy<CharType>(data + old_size, str, size);
      }
      else
      {
        Reserve(size);
        TMemCpy<CharType>(m_Data.l.mem.ptr + size, str, size);
      }

      if (new_size <= m_Data.s.capacity)
        SetShortSize(new_size);
      else
        SetLongSize(new_size);
    }

    void Clear()
    {
      const State state = GetState();
      if (state == State::Long)
        m_Allocator.Deallocate(m_Data.l.mem);
      SetShortSize(0);
    }

    CharType const* CStr() const
    {
      const State state = GetState();
      switch (state)
      {
      case State::Long:
        return m_Data.l.mem.ptr;
      case State::Short:
        return m_Data.s.buffer;
      }
    }

    CharType* Data()
    {
      const State state = GetState();
      switch (state)
      {
      case State::Long:
        return m_Data.l.mem.ptr;
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
        return m_Data.l.mem.size - 1;
      case State::Short:
        return m_Data.s.capacity;
      }
    }

    bool IsEmpty() const
    {
      return Size() == 0;
    }

    //private:

    static constexpr SizeType Length(const CharType* ptr)
    {
      const CharType* s = ptr;
      while (*s != NullTerminator)
        ++s;
      return static_cast<SizeType>(s - ptr);
    }

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
      m_Data.l.mem.ptr[size] = NullTerminator;
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
        MemoryBlock<CharType, SizeType> mem;
        SizeType size;
        uint8 padding; //this is ONLY for the calculation of the final size !
      } l;

      struct ShortStr//this struct holds all the data for the small string which fit on the stack, we cannot easily set the size like in the long representation, so we need the size() function
      {
        static constexpr size_t capacity = (sizeof(LongStr) - sizeof(CharType)) / sizeof(CharType);
        CharType buffer[capacity] = {0};
        CharType state = (capacity << 2);
      } s{};

      uint8 raw[sizeof(LongStr) / sizeof(CharType)];

    } m_Data;

    [[no_unique_address]] AllocType m_Allocator;

  };

  using String = TString<char, '\0'>;
  using WString = TString<wchar_t, L'\0'>;
}
