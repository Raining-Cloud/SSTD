#include "Containers/Pointer.h"

#include <gtest/gtest.h>

namespace SSTDTest
{
  class TestClass
  {
  };

  TEST(SharedPointer, Constructor)
  {
    SSTD::SharedPointer<TestClass> ptr;

    EXPECT_FALSE(ptr.IsValid());
  }

  TEST(SharedPointer, PtrConstructor)
  {
    SSTD::SharedPointer<TestClass> ptr(new TestClass());

    EXPECT_TRUE(ptr.IsValid());
    EXPECT_EQ(ptr.GetRefCount(), 1);
  }

  TEST(SharedPointer, CopyConstructor)
  {
    SSTD::SharedPointer<TestClass> ptr(new TestClass());
    SSTD::SharedPointer<TestClass> ptr2(ptr);

    EXPECT_TRUE(ptr.IsValid());
    EXPECT_TRUE(ptr2.IsValid());
    EXPECT_EQ(ptr.GetRefCount(), 2);
    EXPECT_EQ(ptr2.GetRefCount(), 2);
  }

  TEST(SharedPointer, CopyAssignOperator)
  {
    SSTD::SharedPointer<TestClass> ptr(new TestClass());
    SSTD::SharedPointer<TestClass> ptr2;
    ptr2 = ptr;

    EXPECT_TRUE(ptr.IsValid());
    EXPECT_TRUE(ptr2.IsValid());
    EXPECT_EQ(ptr.GetRefCount(), 2);
    EXPECT_EQ(ptr2.GetRefCount(), 2);
  }

  TEST(SharedPointer, Release)
  {
    SSTD::SharedPointer<TestClass> ptr(new TestClass());
    ptr.Release();

    EXPECT_FALSE(ptr.IsValid());
    EXPECT_EQ(ptr.GetRefCount(), 0);
  }

  TEST(SharedPointer, RefCount)
  {
    SSTD::SharedPointer<TestClass> ptr(new TestClass());
    SSTD::SharedPointer<TestClass> ptr2(ptr);
    SSTD::SharedPointer<TestClass> ptr3(ptr);
    SSTD::SharedPointer<TestClass> ptr4(ptr);

    {
      SSTD::SharedPointer<TestClass> ptr5(ptr);
      EXPECT_EQ(ptr4.GetRefCount(), 5);
    }


    EXPECT_TRUE(ptr.IsValid());
    EXPECT_TRUE(ptr2.IsValid());
    EXPECT_TRUE(ptr3.IsValid());
    EXPECT_TRUE(ptr4.IsValid());

    EXPECT_EQ(ptr.GetRefCount(), 4);
    EXPECT_EQ(ptr2.GetRefCount(), 4);
    EXPECT_EQ(ptr3.GetRefCount(), 4);
    ptr.Release();
    EXPECT_EQ(ptr4.GetRefCount(), 3);
  }

  TEST(SharedPointer, SelfAssign)
  {
    SSTD::SharedPointer<TestClass> ptr(new TestClass());
    ptr = ptr;

    EXPECT_TRUE(ptr.IsValid());
    EXPECT_EQ(ptr.GetRefCount(), 1);
  }



  TEST(UniquePtr, Constructor)
  {
    SSTD::UniquePointer<TestClass> ptr;

    EXPECT_FALSE(ptr.IsValid());
  }

  TEST(UniquePtr, PtrConstructor)
  {
    SSTD::UniquePointer<TestClass> ptr(new TestClass());

    EXPECT_TRUE(ptr.IsValid());
  }

  TEST(UniquePtr, MoveConstructor)
  {
    SSTD::UniquePointer<TestClass> ptr(new TestClass());
    SSTD::UniquePointer<TestClass> ptr2(Move(ptr));

    EXPECT_FALSE(ptr.IsValid());
    EXPECT_TRUE(ptr2.IsValid());
  }

  TEST(UniquePtr, MoveAssignOperator)
  {
    SSTD::UniquePointer<TestClass> ptr(new TestClass());
    SSTD::UniquePointer<TestClass> ptr2;
    ptr2 = Move(ptr);

    EXPECT_FALSE(ptr.IsValid());
    EXPECT_TRUE(ptr2.IsValid());
  }

  TEST(UniquePtr, Release)
  {
    SSTD::UniquePointer<TestClass> ptr(new TestClass());
    ptr.Release();

    EXPECT_FALSE(ptr.IsValid());
  }

  TEST(UniquePtr, ArrowOperator)
  {
    SSTD::UniquePointer<TestClass> ptr(new TestClass());
    TestClass* raw = ptr.operator->();

    EXPECT_EQ(raw, ptr.Get());
  }

  TEST(UniquePtr, StarOperator)
  {
    SSTD::UniquePointer<TestClass> ptr(new TestClass());
    TestClass& obj = ptr.operator*();

    EXPECT_EQ(&obj, ptr.Get());
  }
}