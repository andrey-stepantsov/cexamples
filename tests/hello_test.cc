#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(PointerTest, BasicAssertions) {
	int a = 2;
	int *pa = &a;
	EXPECT_EQ(a, *pa);
	EXPECT_EQ(a, 2);
	EXPECT_EQ(*pa, 2);
}

