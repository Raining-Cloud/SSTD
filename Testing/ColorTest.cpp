#include "Containers/Color.h"

#include <gtest/gtest.h>

namespace SSTDTest
{
  TEST(Color, EmptyConstructor)
  {
    SSTD::Color8 c8{};

    EXPECT_EQ(c8.r, 0);
    EXPECT_EQ(c8.g, 0);
    EXPECT_EQ(c8.b, 0);
    EXPECT_EQ(c8.a, 0);
  }

  TEST(Color, Constructor)
  {
    SSTD::Color8 c8{ 38, 38, 38, 1 };

    EXPECT_EQ(c8.r, 38);
    EXPECT_EQ(c8.g, 38);
    EXPECT_EQ(c8.b, 38);
    EXPECT_EQ(c8.a, 1);
  }

  TEST(Color, Equal)
  {
    SSTD::Color8 c8a{ 38, 38, 38, 0 };
    SSTD::Color8 c8b{ 38, 38, 38, 0 };

    EXPECT_TRUE(c8a == c8b);
  }
}