#include <iostream>
#include <typeinfo>
#include <vector>
#include <gtest/gtest.h>

#include "koala/boxed_value.hpp"

using namespace koala;

TEST(BoxedValue, string) {
  BoxedValue bv = var(123);
  EXPECT_EQ(bv.is_const(), false);
  EXPECT_EQ(bv.is_undef(), false);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}