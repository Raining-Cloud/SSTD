#include <gtest/gtest.h>

#include "Containers/Vector.h"

namespace SSTDTest
{
  TEST(Vector, EmptyConstructor)
  {
    SSTD::Vector<int> v{};

    EXPECT_EQ(v.Size(), 0);
  }

  TEST(Vector, ReserveConstructor)
  {
    SSTD::Vector<int> v{16};

    EXPECT_GE(v.Capacity(), 16);
  }

  TEST(Vector, FillConstructor)
  {
    SSTD::Vector<bool> v{true, 16};

    EXPECT_EQ(v.Size(), 16);
    EXPECT_TRUE(v.Front());
    EXPECT_TRUE(v.Back());
  }

  TEST(Vector, RawConstructor)
  {
    const bool arr[4] = { true, false, true, false };
    SSTD::Vector<bool> v{arr,4};

    EXPECT_EQ(v.Size(), 4);
    EXPECT_TRUE(v.Front());
    EXPECT_FALSE(v.Back());
  }

  TEST(Vector, CopyConstructor)
  {
    SSTD::Vector<bool> v1{true, 4};
    SSTD::Vector<bool> v2{v1};

    EXPECT_EQ(v1.Size(), v2.Size());
    EXPECT_NE(&v1, &v2);
    EXPECT_EQ(v1.Front(), v2.Front());
    EXPECT_EQ(v1.Back(), v2.Back());
  }

  TEST(Vector, MoveConstructor)
  {
    SSTD::Vector<bool> v1{ true, 4 };
    SSTD::Vector<bool> v2{ SSTD::Move(v1) };

    EXPECT_EQ(v1.Size(), 0);
    EXPECT_EQ(v1.Data(), nullptr);
    EXPECT_EQ(v1.Capacity(), 0);
    
    EXPECT_EQ(v2.Size(), 4);
    EXPECT_TRUE(v2.Front());
    EXPECT_TRUE(v2.Back());
  }


}