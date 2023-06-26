#include "Containers/String.h"

#include "gtest/gtest.h"

namespace SSTDTest
{
  struct ReturnString
  {
    SSTD::String Get() const
    {
      return SSTD::String("Hello"); 
    }
  };

  TEST(String, Constructor)
  {
    SSTD::String s;

    ReturnString r;
    EXPECT_TRUE(r.Get() == "Hello");

    EXPECT_EQ(s.Size(), 0);
  }

  TEST(String, SmallConstructor)
  {
    SSTD::String s("HelloWorld");

    EXPECT_EQ(s.Size(), 10);
    EXPECT_GE(s.Capacity(), 10);
  }

  TEST(String, BigConstructor)
  {
    SSTD::String s("Lorem Ipsum is simply dummy text");

    EXPECT_EQ(s.Size(), 32);
    EXPECT_GE(s.Capacity(), 32);
  }

  TEST(String, CopyConstructor)
  {
    SSTD::String s1("HelloWorld");
    SSTD::String s2("Lorem Ipsum is simply dummy text ");

    SSTD::String ss(s1);
    SSTD::String sl(s2);


    EXPECT_EQ(ss.Size(), 10);
    EXPECT_GE(ss.Capacity(), 10);

    EXPECT_EQ(sl.Size(), 33);
    EXPECT_GE(sl.Capacity(), 33);
  }

  TEST(String, Reserve)
  {
    SSTD::String s1("Hello");

    s1.Reserve(10);
    auto cap = s1.Capacity();
    EXPECT_GE(s1.Capacity(), 15);
    EXPECT_EQ(s1.Size(), 5); 

    s1.Reserve(100);
    cap = s1.Capacity();
    EXPECT_GE(s1.Capacity(), 105);
    EXPECT_EQ(s1.Size(), 5);

    s1.Reserve(1000);
    EXPECT_GE(s1.Capacity(), 1005);
    EXPECT_EQ(s1.Size(), 5);

  }

  TEST(String, Minimize)
  {
    SSTD::String s1("Hello");
    s1.Reserve(60);
    EXPECT_GE(s1.Capacity(), 65);
    s1.Minimize();
    EXPECT_EQ(s1.Capacity(), 5);
    EXPECT_EQ(s1.Size(), 5);
  }

  TEST(String, EqualOperator)
  {
    SSTD::String s1("Hello");
    SSTD::String s2("Hello");
    SSTD::String s3("World");

    EXPECT_TRUE(s1 == s2);
    EXPECT_FALSE(s1 == s3);
  }

  TEST(String, Append)
  {
    SSTD::String s("Hello");
    s.Append("World");

    EXPECT_EQ(10, s.Size());
    EXPECT_TRUE(s == "HelloWorld");
  }
}