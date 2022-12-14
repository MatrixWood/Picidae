#include "picidae/lightweighttest/lightweighttest.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

TEST(lightweighttest, array1) {
  int x[] = {1};
  int y[] = {1, 2};
  PICIDAE_TEST_ALL_EQ(x, x + sizeof(x) / sizeof(x[0]), y,
                      y + sizeof(y) / sizeof(y[0]));
}

TEST(lightweighttest, array2) {
  int x[] = {1, 2, 3, 4};
  int y[] = {1, 3, 2, 4};
  PICIDAE_TEST_ALL_EQ(x, x + sizeof(x) / sizeof(x[0]), y,
                      y + sizeof(y) / sizeof(y[0]));
}

TEST(lightweighttest, array3) {
  int x[] = {1, 2, 3, 4};
  int y[] = {1, 2, 3, 4};
  PICIDAE_TEST_ALL_EQ(x, x + sizeof(x) / sizeof(x[0]), y,
                      y + sizeof(y) / sizeof(y[0]));
}

TEST(lightweighttest, vector1) {
  std::vector<int> x, y;
  x.push_back(1);
  x.push_back(2);
  x.push_back(3);
  x.push_back(4);
  y.push_back(1);
  y.push_back(3);
  y.push_back(2);
  y.push_back(4);
  PICIDAE_TEST_ALL_EQ(x.begin(), x.end(), y.begin(), y.end());
}

TEST(lightweighttest, test_all_with_impl_1) {
  std::vector<int> x, y;
  x.push_back(1);
  PICIDAE_TEST_ALL_WITH(x.begin(), x.end(), y.begin(), y.end(),
                        std::equal_to<int>());
}

TEST(lightweighttest, test_all_with_impl_2) {
  std::vector<int> x, y;
  x.push_back(1);
  x.push_back(2);
  x.push_back(3);
  x.push_back(4);
  y.push_back(1);
  y.push_back(2);
  y.push_back(3);
  PICIDAE_TEST_ALL_WITH(x.begin(), x.end(), y.begin(), y.end(),
                        std::equal_to<int>());
}

TEST(lightweighttest, bool) {
  PICIDAE_TEST(PICIDAE_TEST(true));
  PICIDAE_TEST_NOT(PICIDAE_TEST(false));

  PICIDAE_TEST(PICIDAE_TEST_NOT(false));
  PICIDAE_TEST_NOT(PICIDAE_TEST_NOT(true));

  PICIDAE_TEST(PICIDAE_TEST_EQ(1, 1));
  PICIDAE_TEST_NOT(PICIDAE_TEST_EQ(1, 2));

  PICIDAE_TEST(PICIDAE_TEST_NE(1, 2));
  PICIDAE_TEST_NOT(PICIDAE_TEST_NE(1, 1));

  PICIDAE_TEST(PICIDAE_TEST_LT(1, 2));
  PICIDAE_TEST_NOT(PICIDAE_TEST_LT(1, 1));

  PICIDAE_TEST(PICIDAE_TEST_LE(1, 1));
  PICIDAE_TEST_NOT(PICIDAE_TEST_LE(2, 1));

  PICIDAE_TEST(PICIDAE_TEST_GT(2, 1));
  PICIDAE_TEST_NOT(PICIDAE_TEST_GT(1, 1));

  PICIDAE_TEST(PICIDAE_TEST_GE(1, 1));
  PICIDAE_TEST_NOT(PICIDAE_TEST_GE(1, 2));

  PICIDAE_TEST(PICIDAE_TEST_CSTR_EQ("1", "1"));
  PICIDAE_TEST_NOT(PICIDAE_TEST_CSTR_EQ("1", "2"));

  PICIDAE_TEST(PICIDAE_TEST_CSTR_NE("1", "2"));
  PICIDAE_TEST_NOT(PICIDAE_TEST_CSTR_NE("1", "1"));
}

TEST(lightweighttest, report_errors) {
  picidae::lightweighttest::report_errors();
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}