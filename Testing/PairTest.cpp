#include "Containers/Pair.h"

#include <gtest/gtest.h>

namespace SSTDTest
{
  TEST(Pair, Constructor)
  {
    SSTD::Pair<int, bool> p(200, false);

    EXPECT_EQ(p.first, 200);
    EXPECT_EQ(p.second, false);
  }

  TEST(Pair, EqualOperator)
  {
    SSTD::Pair<int, bool> p(200, true);
    SSTD::Pair<int, bool> p2(200, true);

    EXPECT_TRUE(p == p2);

    p2.first = 0;

    EXPECT_FALSE(p == p2);

    p2 = p;

    EXPECT_TRUE(p == p2);
  }
}