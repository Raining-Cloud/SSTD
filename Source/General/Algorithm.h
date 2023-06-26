#pragma once
#include "General/Numeric.h"
#include "General/Utility.h"

namespace SSTD
{
  namespace SortType
  {
    struct Insertion
    {
      template<typename ContainerIterator, typename CompareFunction>
      constexpr void Sort(ContainerIterator begin, ContainerIterator end, CompareFunction func)
      {
        if (begin == end)
          return;

        for (auto i = begin + 1; i < end; ++i)
        {
          auto key = *i;
          auto j = i - 1;
          while (j >= begin && func(*j, key))
          {
            *(j + 1) = *j;
            --j;
          }
          *(j + 1) = key;
        }
      }

      static constexpr bool stable = true;
    };
  }

  template<class SortType = SortType::Insertion, class ContainerIterator, class CompareFunction>
  static constexpr void Sort(ContainerIterator first, ContainerIterator last, CompareFunction cond)
  {
    auto s = SortType(); s.Sort(first, last, cond);
  }

  template<class SortType = SortType::Insertion, class ContainerIterator>
  static constexpr void Sort(ContainerIterator first, ContainerIterator last)
  {
    auto s= SortType(); s.Sort(first, last,
      [](auto a, auto b) {
        return a < b;
      });
  }

  template<typename SrcIterator, typename DstIterator, typename Size>
  static constexpr DstIterator Copy(SrcIterator src, DstIterator dst, Size n)
  {
    *(dst++) = *src;
    for (Size i = 0; i < n; ++i)
      *dst++ = *(++src);
    return dst;
  }
}