#include <iostream>
#include <typeinfo>
#include <vector>
#include <gtest/gtest.h>

#include "koala/function_params.hpp"
#include "koala/function_signature.hpp"

using namespace koala;

TEST(FunctionParams, base) {
  std::vector<BoxedValue> vec_bv;
  for (int i = 0; i < 5; ++i) {
    BoxedValue bv(i);
    vec_bv.push_back(bv);
  }

  FunctionParams fp(vec_bv);

  EXPECT_EQ(vec_bv.begin()->is_const(), false);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}