#include "Containers/Rect.h"

#include <gtest/gtest.h>

namespace SSTDTest
{
  TEST(Rect, EmptyConstructor)
  {
    SSTD::Rect<int> r{};

    EXPECT_EQ(r.left, 0);
    EXPECT_EQ(r.right, 0);
    EXPECT_EQ(r.top, 0);
    EXPECT_EQ(r.bottom, 0);
  }

  TEST(Rect, ListAssignment)
  {
    SSTD::Rect<int> r = { 1,2,3,4 };

    EXPECT_EQ(r.left, 1);
    EXPECT_EQ(r.top, 2);
    EXPECT_EQ(r.right, 3);
    EXPECT_EQ(r.bottom, 4);
  }

  TEST(Rect, Constructor)
  {
    SSTD::Rect<int> r(1,2,3,4);

    EXPECT_EQ(r.left, 1);
    EXPECT_EQ(r.top, 2);
    EXPECT_EQ(r.right, 3);
    EXPECT_EQ(r.bottom, 4);
  }

  TEST(Rect, EqualOperator)
  {
    SSTD::Rect<int> r(1, 2, 3, 4);
    SSTD::Rect<int> r2(1, 2, 3, 4);
    SSTD::Rect<int> r3(1, 2, 6, 4);

    EXPECT_TRUE(r == r2);

    EXPECT_FALSE(r3 == r);
    EXPECT_FALSE(r3 == r2);
  }

  TEST(Rect, Width)
  {
    SSTD::Rect<int> r(0,0,1920,1080);
    SSTD::Rect<int> r2;

    EXPECT_EQ(r.Width(), 1920);
    EXPECT_EQ(r2.Width(), 0);
  }

  TEST(Rect, Height)
  {
    SSTD::Rect<int> r(0, 0, 1920, 1080);
    SSTD::Rect<int> r2;

    EXPECT_EQ(r.Height(), 1080);
    EXPECT_EQ(r2.Height(), 0);
  }
}