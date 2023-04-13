#include <gtest/gtest.h>

#include "picidae/core/any.hpp"

TEST(Any, ConstructionAndAssignment) {
  using namespace picidae;
  using namespace picidae::core;

  Any A;
  Any B{7};
  Any C{8};
  Any D{"hello"};
  Any E{3.7};

  // An empty Any is not anything.
  EXPECT_FALSE(A.has_value());
  EXPECT_FALSE(any_cast<int>(&A));

  // An int is an int but not something else.
  EXPECT_TRUE(B.has_value());
  EXPECT_TRUE(any_cast<int>(&B));
  EXPECT_FALSE(any_cast<float>(&B));

  EXPECT_TRUE(C.has_value());
  EXPECT_TRUE(any_cast<int>(&C));

  // A const char * is a const char * but not an int.
  EXPECT_TRUE(D.has_value());
  EXPECT_TRUE(any_cast<const char *>(&D));
  EXPECT_FALSE(any_cast<int>(&D));

  // A double is a double but not a float.
  EXPECT_TRUE(E.has_value());
  EXPECT_TRUE(any_cast<double>(&E));
  EXPECT_FALSE(any_cast<float>(&E));

  // After copy constructing from an int, the new item and old item are both
  // ints.
  Any F(B);
  EXPECT_TRUE(B.has_value());
  EXPECT_TRUE(F.has_value());
  EXPECT_TRUE(any_cast<int>(&F));
  EXPECT_TRUE(any_cast<int>(&B));

  // After move constructing from an int, the new item is an int and the old one
  // isn't.
  Any G(std::move(C));
  EXPECT_FALSE(C.has_value());
  EXPECT_TRUE(G.has_value());
  EXPECT_TRUE(any_cast<int>(&G));
  EXPECT_FALSE(any_cast<int>(&C));

  // After copy-assigning from an int, the new item and old item are both ints.
  A = F;
  EXPECT_TRUE(A.has_value());
  EXPECT_TRUE(F.has_value());
  EXPECT_TRUE(any_cast<int>(&A));
  EXPECT_TRUE(any_cast<int>(&F));

  // After move-assigning from an int, the new item and old item are both ints.
  B = std::move(G);
  EXPECT_TRUE(B.has_value());
  EXPECT_FALSE(G.has_value());
  EXPECT_TRUE(any_cast<int>(&B));
  EXPECT_FALSE(any_cast<int>(&G));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}