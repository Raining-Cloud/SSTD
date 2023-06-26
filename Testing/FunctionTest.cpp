#include "Containers/Function.h"

#include <gtest/gtest.h>

namespace SSTDTest
{
  uint32 AddGlobal(uint32 a, uint32 b) { return a + b; }

  class TestClass
  {
  public:
    uint32 AddConst(uint32 a) const { return member + a; }
    uint32 Add(uint32 a) { return member += a; }
    static uint32 AddStatic(uint32 a, uint32 b) { return a + b; }
  private:
    uint32 member{ 10 };
  };

  TEST(Function, Constructor)
  {
    SSTD::Function<void()> f;

    EXPECT_FALSE(f.IsBound());
  }

  TEST(Function, Static)
  {
    TestClass c;
    SSTD::Function<uint32(uint32, uint32)> f;
    f.Bind<TestClass::AddStatic>();
    SSTD::Function<uint32(uint32, uint32)> f2(f);


    EXPECT_EQ(f(20, 10), 30);
    EXPECT_EQ(f2(20, 10), 30);
  }

  TEST(Function, Global)
  {
    TestClass c;
    SSTD::Function<uint32(uint32, uint32)> f;
    f.Bind<AddGlobal>();
    SSTD::Function<uint32(uint32, uint32)> f2(f);

    EXPECT_EQ(f(20, 10), 30);
    EXPECT_EQ(f2(20, 10), 30);
  }

  TEST(Function, Lambda)
  {
    auto lambda = [](uint32 a, uint32 b) { return a + b; };
    SSTD::Function<uint32(uint32, uint32)> f;
    f.Bind(lambda);
    SSTD::Function<uint32(uint32, uint32)> f2(f);

    EXPECT_EQ(f2(20, 10), 30);
    EXPECT_EQ(f(20, 10), 30);
  }

  TEST(Function, LambdaCapture)
  {
    uint32  val = 42;
    auto lambda = [&val](uint32 a, uint32 b) { return a + b + val; };
    SSTD::Function<uint32(uint32, uint32)> f;
    f.Bind(lambda);
    SSTD::Function<uint32(uint32, uint32)> f2(f);

    EXPECT_EQ(f(20, 10), 72);
    EXPECT_EQ(f2(20, 10), 72);

    val = 10;
    EXPECT_EQ(f(20, 10), 40);
    EXPECT_EQ(f2(20, 10), 40);
  }

  TEST(Function, Member)
  {
    TestClass c;
    SSTD::Function<uint32(uint32)> f;
    f.Bind<TestClass, &TestClass::Add>(&c);
    SSTD::Function<uint32(uint32)> f2(f);

    EXPECT_EQ(f(20), 30);
    EXPECT_EQ(f2(20), 50); //first call adds 20
  }

  TEST(Function, ConstMember)
  {
    TestClass c;
    SSTD::Function<uint32(uint32)> f;
    f.Bind<TestClass, &TestClass::AddConst>(c);
    SSTD::Function<uint32(uint32)> f2(f);

    EXPECT_EQ(f(20), 30);
    
    c.Add(10);
    EXPECT_EQ(f2(20), 40);
  }
}