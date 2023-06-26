#include "Containers/Array.h"

#include <gtest/gtest.h>


namespace SSTDTest
{
  TEST(Array, EmptyConstructor)
  {
    SSTD::Array<int, 8> a;

    EXPECT_EQ(a.Size(), 8);
  }

  TEST(Array, CopyConstructor)
  {
    SSTD::Array<int, 8> a;

    EXPECT_EQ(a.Size(), 8);
  }

  TEST(Array, FillLoop)
  {
    SSTD::Array<int, 8> a;

    int x = 0;
    for (auto& e : a)
      e = x++;

    EXPECT_EQ(a.Size(), 8);
    EXPECT_EQ(a.At(0), 0);
    EXPECT_EQ(a.At(4), 4);
    EXPECT_EQ(a.At(7), 7);
  }
}