#pragma once

#include "Platform/IncludePlatform.h"

#include "General/Numeric.h"

namespace SSTD
{
  namespace AtomicUtils
  {
#ifdef PLATFORM_WIN64

#include <intrin0.h>

    static inline uint8 Load(volatile uint8& ptr)
    {
      return _InterlockedOr8(reinterpret_cast<volatile CHAR*>(&ptr), 0);
    }
    static inline uint16 Load(volatile uint16& ptr)
    {
      return _InterlockedOr16(reinterpret_cast<volatile SHORT*>(&ptr), 0);
    }
    static inline uint32 Load(volatile uint32& ptr)
    {
      return _InterlockedOr(reinterpret_cast<volatile LONG*>(&ptr), 0);
    }
    static inline uint64 Load(volatile uint64& ptr)
    {
      return _InterlockedOr64(reinterpret_cast<volatile LONG64*>(&ptr), 0);
    }


    static inline void Store(volatile uint8& ptr, uint8 value)
    {
      _InterlockedExchange8(reinterpret_cast<volatile CHAR*>(&ptr), value);
    }
    static inline void Store(volatile uint16& ptr, uint16 value)
    {
      _InterlockedExchange16(reinterpret_cast<volatile SHORT*>(&ptr), value);
    }
    static inline void Store(volatile uint32& ptr, uint32 value)
    {
      _InterlockedExchange(reinterpret_cast<volatile LONG*>(&ptr), value);
    }
    static inline void Store(volatile uint64& ptr, uint64 value)
    {
      _InterlockedExchange64(reinterpret_cast<volatile LONG64*>(&ptr), value);
    }

    static inline uint8 PreIncrement(volatile uint8& ptr)
    {
      auto out = _InterlockedExchangeAdd8(reinterpret_cast<volatile CHAR*>(&ptr), 1);
      return ++out;
    }
    static inline uint16 PreIncrement(volatile uint16& ptr)
    {
      return _InterlockedIncrement16(reinterpret_cast<volatile SHORT*>(&ptr));
    }
    static inline uint32 PreIncrement(volatile uint32& ptr)
    {
      return _InterlockedIncrement(reinterpret_cast<volatile LONG*>(&ptr));
    }
    static inline uint64 PreIncrement(volatile uint64& ptr)
    {
      return _InterlockedIncrement64(reinterpret_cast<volatile LONG64*>(&ptr));
    }


    static inline uint8 PostIncrement(volatile uint8& ptr)
    {
      return _InterlockedExchangeAdd8(reinterpret_cast<volatile CHAR*>(&ptr), 1L);
    }
    static inline uint16 PostIncrement(volatile uint16& ptr)
    {
      return _InterlockedExchangeAdd16(reinterpret_cast<volatile SHORT*>(&ptr), 1L);
    }
    static inline uint32 PostIncrement(volatile uint32& ptr)
    {
      return _InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(&ptr), 1L);
    }
    static inline uint64 PostIncrement(volatile uint64& ptr)
    {
      return _InterlockedExchangeAdd64(reinterpret_cast<volatile LONG64*>(&ptr), 1L);
    }


    static inline uint8 PreDecrement(volatile uint8& ptr)
    {
      auto out = _InterlockedExchangeAdd8(reinterpret_cast<volatile CHAR*>(&ptr), -1);
      return --out;
    }
    static inline uint16 PreDecrement(volatile uint16& ptr)
    {
      return _InterlockedDecrement16(reinterpret_cast<volatile SHORT*>(&ptr));
    }
    static inline uint32 PreDecrement(volatile uint32& ptr)
    {
      return _InterlockedDecrement(reinterpret_cast<volatile LONG*>(&ptr));
    }
    static inline uint64 PreDecrement(volatile uint64& ptr)
    {
      return _InterlockedDecrement64(reinterpret_cast<volatile LONG64*>(&ptr));
    }


    static inline uint8 PostDecrement(volatile uint8& ptr)
    {
      return _InterlockedExchangeAdd8(reinterpret_cast<volatile CHAR*>(&ptr), -1L);
    }
    static inline uint16 PostDecrement(volatile uint16& ptr)
    {
      return _InterlockedExchangeAdd16(reinterpret_cast<volatile SHORT*>(&ptr), -1L);
    }
    static inline uint32 PostDecrement(volatile uint32& ptr)
    {
      return _InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(&ptr), -1L);
    }
    static inline uint64 PostDecrement(volatile uint64& ptr)
    {
      return _InterlockedExchangeAdd64(reinterpret_cast<volatile LONG64*>(&ptr), -1L);
    }


    static inline uint8 Add(volatile uint8& ptr, uint8 value)
    {
      return value + _InterlockedExchangeAdd8(reinterpret_cast<volatile CHAR*>(&ptr), value);
    }

    static inline uint16 Add(volatile uint16& ptr, uint16 value)
    {
      return value + _InterlockedExchangeAdd16(reinterpret_cast<volatile SHORT*>(&ptr), value);
    }

    static inline uint32 Add(volatile uint32& ptr, uint32 value)
    {
      return value + _InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(&ptr), value);
    }

    static inline uint64 Add(volatile uint64& ptr, uint64 value)
    {
      return value + _InterlockedExchangeAdd64(reinterpret_cast<volatile LONG64*>(&ptr), value);
    }


    static inline uint8 Subtract(volatile uint8& ptr, uint8 value)
    {
      return Add(ptr, -value);
    }

    static inline uint16 Subtract(volatile uint16& ptr, uint16 value)
    {
      return Add(ptr, -value);
    }

    static inline uint32 Subtract(volatile uint32& ptr, uint32 value)
    {
      return Add(ptr, -value);
    }

    static inline uint64 Subtract(volatile uint64& ptr, uint64 value)
    {
      return Add(ptr, -value);
    }


    static inline uint8 And(volatile uint8& ptr, uint8 value)
    {
      return _InterlockedAnd8(reinterpret_cast<volatile CHAR*>(&ptr), value);
    }
    static inline uint16 And(volatile uint16& ptr, uint16 value)
    {
      return _InterlockedAnd16(reinterpret_cast<volatile SHORT*>(&ptr), value);
    }
    static inline uint32 And(volatile uint32& ptr, uint32 value)
    {
      return _InterlockedAnd(reinterpret_cast<volatile LONG*>(&ptr), value);
    }
    static inline uint64 And(volatile uint64& ptr, uint64 value)
    {
      return _InterlockedAnd64(reinterpret_cast<volatile LONG64*>(&ptr), value);
    }


    static inline uint8 Or(volatile uint8& ptr, uint8 value)
    {
      return _InterlockedOr8(reinterpret_cast<volatile CHAR*>(&ptr), value);
    }
    static inline uint16 Or(volatile uint16& ptr, uint16 value)
    {
      return _InterlockedOr16(reinterpret_cast<volatile SHORT*>(&ptr), value);
    }
    static inline uint32 Or(volatile uint32& ptr, uint32 value)
    {
      return _InterlockedOr(reinterpret_cast<volatile LONG*>(&ptr), value);
    }
    static inline uint64 Or(volatile uint64& ptr, uint64 value)
    {
      return _InterlockedOr64(reinterpret_cast<volatile LONG64*>(&ptr), value);
    }


    static inline uint8 Xor(volatile uint8& ptr, uint8 value)
    {
      return _InterlockedXor8(reinterpret_cast<volatile CHAR*>(&ptr), value);
    }
    static inline uint16 Xor(volatile uint16& ptr, uint16 value)
    {
      return _InterlockedXor16(reinterpret_cast<volatile SHORT*>(&ptr), value);
    }
    static inline uint32 Xor(volatile uint32& ptr, uint32 value)
    {
      return _InterlockedXor(reinterpret_cast<volatile LONG*>(&ptr), value);
    }
    static inline uint64 Xor(volatile uint64& ptr, uint64 value)
    {
      return _InterlockedXor64(reinterpret_cast<volatile LONG64*>(&ptr), value);
    }
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
  }
}