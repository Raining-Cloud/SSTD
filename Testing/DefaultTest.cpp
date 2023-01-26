#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(DefaultTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STREQ("This test is just a default test!", "This test is just a default test!");
}
