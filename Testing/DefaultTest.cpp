#include <gtest/gtest.h>

#include "Math/Vec.h"

namespace SSTDTest
{
  // Demonstrate some basic assertions.
  TEST(DefaultTest, BasicAssertions) {
    // Expect two strings not to be equal.
    const char* str = "This test is just a default test!";
    EXPECT_STREQ(str, str);
  }

  TEST(DevTest, Test)
  {
    EXPECT_EQ(true, true);
  }
}
