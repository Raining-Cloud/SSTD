#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(DefaultTest, BasicAssertions) {
  // Expect two strings not to be equal.
  const char* str = "This test is just a default test!";
  EXPECT_STREQ(str, str);
}
