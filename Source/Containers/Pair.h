#pragma once

#include "Utility.h"

namespace SSTD
{
  template<typename T1, typename T2>
  struct Pair
  {
    Pair() : first(), second() {}
    Pair(const T1& t1, const T2& t2) : first(t1), second(t2) {}
    Pair(T1&& t1, T2&& t2) : first(Move(t1)), second(Move(t2)) {}

    template<class U1, class U2>
    Pair(Pair<U1, U2>&& other) : first(Move(other.first)), second(Move(other.second)) {}

    Pair& operator=(Pair&& other) { first = Move(other.first); second = Move(other.second); return *this; }

    template<class U1, class U2>
    Pair& operator=(Pair<U1, U2>&& other) { first = Move(other.first); second = Move(other.second); return *this; }

    inline bool operator==(const Pair& other) { return first == other.first && second == other.second; }

    T1 first;
    T2 second;
  };
}