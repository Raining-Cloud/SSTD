#pragma once
#include "Numeric.h"
#include "Utility.h"

namespace SSTD
{
  struct SortTypeInsertion 
  {
  public:
    template<class ContainerIterator, typename CompareFunction>
    void Sort(ContainerIterator first, ContainerIterator last, CompareFunction func)
    {
      if (first == last)
        return;

      auto cur = first;
      cur++;

      //iterate all elements!
      while (cur != last)
      {
        auto comp = Move(*cur);

        auto pos = cur;
        //check until we found n-1 < n < n+1
        while (pos != first)
        {
          auto left = pos;
          --left;
          if (!func(comp, *left))
            break;

          *pos = Move(*left);
          pos = Move(left);
        }
        if (pos != cur)
          *pos = Move(comp);
        cur++;
      }
    }

    template<typename SizeType = uint32>
    SizeType getComplexity(SizeType n) const { return n ^ 2; }

    const bool isStable = false;
  protected:
  };

  template<class SortType = SortTypeInsertion, class ContainerIterator, class CompareFunction>
  static constexpr void Sort(ContainerIterator first, ContainerIterator last, CompareFunction cond)
  {
    auto s = SortType(); s.Sort(first, last, cond);
  }

  template<class SortType = SortTypeInsertion, class ContainerIterator>
  static constexpr void Sort(ContainerIterator first, ContainerIterator last)
  {
    auto s= SortType(); s.Sort(first, last,
      [](auto a, auto b) {
        return a < b;
      });
  }

  struct HashTypeMD5
  {

  };

  template<class HashType = HashTypeMD5>
  static constexpr void Hash()
  {

  }
}