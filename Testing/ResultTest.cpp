#include <gtest/gtest.h>

#include "General/Result.h"
#include "General/Utility.h"

namespace SSTDTest
{
  struct Error { int code = 42; };

  SSTD::Result<int, Error> OkResult()
  {
    return SSTD::Result<int, Error>(1);
  }

  SSTD::Result<int, Error> ErrorResult()
  {
    return SSTD::Result<int, Error>(Error());
  }

  TEST(Result, Constructor)
  {
    SSTD::Result<int, Error> r1(1);
    SSTD::Result<int, Error> r2(Error{});

    EXPECT_TRUE(r1.Ok());
    EXPECT_TRUE(r2.Error());
  }

  TEST(Result, Copy)
  {
    auto r1 = OkResult();
    auto r2 = ErrorResult();

    EXPECT_TRUE(r1.Ok());
    EXPECT_TRUE(r2.Error());
  }

  TEST(Result, Move)
  {
    SSTD::Result<int, Error> r1{ SSTD::Move(OkResult()) };
    SSTD::Result<int, Error> r2{ SSTD::Move(ErrorResult()) };

    EXPECT_TRUE(r1.Ok());
    EXPECT_TRUE(r2.Error());
  }

  TEST(Result, Matching)
  {
    EXPECT_EQ(
      OkResult().Match(
        [](int i) { return i; },
        [](Error e) { return e.code; }
      ),
      1
    );

    EXPECT_EQ(
      ErrorResult().Match(
        [](int i) { return i; },
        [](Error e) { return e.code; }
      ),
      42
    );
  }
}