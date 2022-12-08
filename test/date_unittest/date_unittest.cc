#include "picidae/date/date.hpp"

#include <sstream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "picidae/lightweighttest/lightweighttest.hpp"

void test() {
  using namespace picidae::date;
  auto d = 1_d;

  PICIDAE_TEST(++d == 2_d);
  PICIDAE_TEST(d++ == 2_d);
  PICIDAE_TEST(d == 3_d);
  PICIDAE_TEST(d-- == 3_d);
  PICIDAE_TEST(d == 2_d);
  PICIDAE_TEST(--d == 1_d);
  PICIDAE_TEST((d += days{2}) == 3_d);
  PICIDAE_TEST((d -= days{2}) == 1_d);

  auto day = 1_d;
  std::ostringstream oss;
  oss << day;
  std::cout << oss.str() << std::endl;
  PICIDAE_TEST(oss.str() == "01");

  d += days{11};
  oss.str("");
  oss << d;
  std::cout << oss.str() << std::endl;
  PICIDAE_TEST(oss.str() == "12");
}

void test_month() {
  using namespace picidae::date;

  auto months_15 = months{-15};

  auto month_1 = 1_m;
  auto month_13 = 13_m;
  auto month_12 = 12_m;
  auto plus_m_ms = month_1 + months_15;
  auto minus_m_m = month_13 - month_1;
  auto plus_m_m = month_13 + month_1;
  std::cout << unsigned(plus_m_ms) << std::endl;  // 10
  std::cout << minus_m_m.count() << std::endl;    // 12
  std::cout << unsigned(month_13) << std::endl;   // 1
  std::cout << unsigned(month_12) << std::endl;   // 12
  std::cout << plus_m_m.count() << std::endl;     // 2

  std::cout << plus_m_ms << std::endl;  // 10
  std::cout << month_13 << std::endl;   // 1
  std::cout << month_12 << std::endl;   // 12

  static_assert(jan == month{1}, "");
  static_assert(feb == month{2}, "");
  static_assert(mar == month{3}, "");
  static_assert(apr == month{4}, "");
  static_assert(may == month{5}, "");
  static_assert(jun == month{6}, "");
  static_assert(jul == month{7}, "");
  static_assert(aug == month{8}, "");
  static_assert(sep == month{9}, "");
  static_assert(oct == month{10}, "");
  static_assert(nov == month{11}, "");
  static_assert(dec == month{12}, "");
}

int main(int argc, char **argv) {
  test();
  test_month();
  picidae::lightweighttest::report_errors();

  return 0;
}