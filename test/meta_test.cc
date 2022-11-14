#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "picidae/cpplisp/cpplisp.hpp"
#include "picidae/meta/common.hpp"
#include "picidae/meta/debug.hpp"
#include "picidae/meta/descr.hpp"
#include "picidae/meta/detail/mt_fold.hpp"
#include "picidae/meta/detail/mt_plus.hpp"
#include "picidae/meta/detail/mt_list.hpp"

TEST(Meta, common) {
  using namespace picidae;
  using namespace picidae::meta;
/*
  EXPECT_EQ(e<char>, '\x2');
  EXPECT_EQ(e<int>, 2);
  EXPECT_DOUBLE_EQ(e<double>, 2.7182818284590451);
  EXPECT_FLOAT_EQ(e<float>, 2.7182817);

  EXPECT_EQ(bool_<false>::value, 0);
  EXPECT_EQ(index_t<5>::value, 5);

  var c = cpplisp::runtime::cons(1, cpplisp::runtime::nil);
  execute<c_1::value, 1>();
  execute<_typeof<cpplisp::runtime::_list_t<int>>::type,
          cpplisp::runtime::ConsPtr<int, cpplisp::runtime::nil_t>>();
*/
#if __cplusplus >= 202002L
  execute<is_base_template_of_v<std::tuple, std::tuple<int, char>>>();
#endif
}

TEST(Meta, mt_plus) {
  using namespace picidae;
  using namespace picidae::meta;

  execute<std::is_same<mt_plus<>, std::integral_constant<int, 0>>::value,
          true>();
  execute<std::is_same<mt_plus<std::integral_constant<int, 0>,
                               std::integral_constant<int, 1>>,
                       std::integral_constant<int, 1>>::value,
          true>();

  execute<std::is_same<mt_plus<std::integral_constant<char, 0>,
                               std::integral_constant<char, 1>>,
                       std::integral_constant<int, 1>>::value,
          true>();
}

struct X1 {};
struct X2 {};
struct X3 {};

template <class T1, class T2>
struct F {};

TEST(Meta, mt_fold) {
  using namespace picidae;
  using namespace picidae::meta;

  execute<mt_fold<mt_list<>, void, F>, void>();
  execute<mt_fold<mt_list<X1>, void, F>, F<void, X1>>();
  execute<mt_fold<mt_list<X1, X2>, void, F>, F<F<void, X1>, X2>>();
  execute<mt_fold<mt_list<X1, X2, X3>, void, F>, F<F<F<void, X1>, X2>, X3>>();
}

TEST(Meta, descr) {
  using namespace picidae;

  const char text[] = "testtext";
  const char text1[] = "testtext1";
  auto t = meta::const_name(text);

  auto t2 = meta::_<true>(text, text1);
  //std::cout << t2.text << std::endl;
  meta::_<12>();
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}