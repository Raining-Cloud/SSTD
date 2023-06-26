#pragma once

#include "General/Utility.h"

namespace SSTD
{
  template<typename T1, typename T2>
  struct Pair
  {
    Pair() noexcept {}
    Pair(const Pair& other) : first(other.first), second(other.second) {}
    Pair(Pair&& other) noexcept : first(Move(other.first)), second(Move(other.second)) {}
    Pair(const T1& t1, const T2& t2) :first(t1), second(t2) {}

    template<class U1, class U2>
    Pair(Pair<U1, U2>&& other) : first(Move(other.first)), second(Move(other.second)) {}

    Pair& operator=(Pair&& other) noexcept { first = Move(other.first); second = Move(other.second); return *this; }

    Pair& operator=(const Pair& other) noexcept { first = other.first; second = other.second; return *this; }

    template<class U1, class U2>
    Pair& operator=(Pair<U1, U2>&& other) { first = Move(other.first); second = Move(other.second); return *this; }

    bool operator==(const Pair& other) { return first == other.first && second == other.second; }
    bool operator!=(const Pair& other) { return !(*this == other); }

    T1 first{};
    T2 second{};
  };
}