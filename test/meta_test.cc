#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "picidae/meta/debug.hpp"
#include "picidae/meta/meta.hpp"

using namespace picidae;

TEST(Meta, meta) {
  EXPECT_EQ(e<char>, '\x2');
  EXPECT_EQ(e<int>, 2);
  EXPECT_DOUBLE_EQ(e<double>, 2.7182818284590451);
  EXPECT_FLOAT_EQ(e<float>, 2.7182817);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}