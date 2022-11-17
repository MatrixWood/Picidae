#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "picidae/json/json.hpp"

TEST(Json, base) {
  using namespace picidae::json;

  auto data = Json::Load("\"12\"");
  auto str = data.dump();

  std::cout << str << std::endl;
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}