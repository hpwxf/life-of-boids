#include <cmath>
#include <iostream>
#include "gtest/gtest.h"

// blank namespace to avoid implementation collision
namespace {

TEST(FirstTest, Canary) {
  ASSERT_EQ(1, 1) << "Canary could not fail!";
}

// Disabled by name
TEST(DISABLED_FirstTest, Feature2) {
  ASSERT_EQ(1, 2);
}
TEST(FirstTest, DISABLED_Feature3) {
  ASSERT_EQ(1, 2);
}

}  // namespace
