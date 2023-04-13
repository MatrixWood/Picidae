#include <iostream>
#include <typeinfo>
#include <vector>
#include <gtest/gtest.h>

#include "koala/boxed_number.hpp"

using namespace koala;

TEST(BoxedNumber, get_common_type) {
  // EXPECT_EQ(BoxedNumber::get_common_type(1, true), BoxedNumber::CommonTypes::T_INT8);
  // EXPECT_EQ(BoxedNumber::get_common_type(1, false), BoxedNumber::CommonTypes::T_UINT8);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}