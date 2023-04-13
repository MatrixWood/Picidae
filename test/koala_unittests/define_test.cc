#include <iostream>
#include <typeinfo>
#include <vector>
#include <gtest/gtest.h>

#include "koala/define.hpp"

using namespace koala;

TEST(StrEqual, string) {
  std::string str1 = "hello";
  std::string str2 = "hello";
  std::string str3 = "Hello";
  StrEqual str_equal;
  EXPECT_EQ(str_equal(str1, str2), true);
  EXPECT_EQ(str_equal(str1, str3), false);
}

TEST(StrEqual, vector) {
  std::vector<int> vec1{1, 2, 3};
  std::vector<int> vec2{1, 2, 3};
  std::vector<int> vec3{1, 2, 3, 4, 5};
  StrEqual str_equal;
  EXPECT_EQ(str_equal(vec1, vec2), true);
  EXPECT_EQ(str_equal(vec1, vec3), false);
}

TEST(MakeVector, null) {
  std::vector<int> vec1{1, 2, 3};
  std::vector<int> vec2{1, 2, 3};
  auto vec3 = make_vector(1, 2, 3);
  StrEqual str_equal;
  EXPECT_EQ(str_equal(vec1, vec2), true);
  EXPECT_EQ(str_equal(vec1, vec3), true);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}