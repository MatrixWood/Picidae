#include "picidae/date/date.hpp"

#include <cassert>
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

  static_assert(January == month{1}, "");
  static_assert(February == month{2}, "");
  static_assert(March == month{3}, "");
  static_assert(April == month{4}, "");
  static_assert(May == month{5}, "");
  static_assert(June == month{6}, "");
  static_assert(July == month{7}, "");
  static_assert(August == month{8}, "");
  static_assert(September == month{9}, "");
  static_assert(October == month{10}, "");
  static_assert(November == month{11}, "");
  static_assert(December == month{12}, "");
}

void test_year() {
  using namespace picidae::date;

  auto y2015 = 2015_y;

  static_assert(year{2015} == 2015_y, "");
  static_assert(year{2015} != 2016_y, "");
  static_assert(year{2015} < 2016_y, "");
  static_assert(year{2016} > 2015_y, "");
  static_assert(year{2015} <= 2015_y, "");
  static_assert(year{2016} >= 2015_y, "");

  static_assert(!year{2015}.is_leap(), "");
  static_assert(year{2016}.is_leap(), "");

  static_assert(year::min().ok(), "");
  static_assert(year{2015}.ok(), "");
  static_assert(year{2016}.ok(), "");
  static_assert(year::max().ok(), "");

  static_assert(2015_y - 2010_y == years{5}, "");
  static_assert(2015_y - years{5} == 2010_y, "");
  static_assert(2015_y == years{5} + 2010_y, "");
  static_assert(2015_y == 2010_y + years{5}, "");

  auto y = 2015_y;
  std::ostringstream os;
  os << y;
  assert(os.str() == "2015");
}

void test_weekday() {
  using namespace picidae::date;

  static_assert(!(sun != sun), "");
  static_assert(sun != mon, "");
  static_assert(mon != sun, "");

  std::ostringstream os;
  os << sun;
  std::cout << os.str() << std::endl;

  std::cout << (weekday{6} - weekday{1}).count() << std::endl;
}

void test_weekday_indexed() {
  using namespace picidae::date;

  constexpr weekday_indexed wdi = sun[1];
  static_assert(wdi.weekday() == sun, "");
  static_assert(wdi.index() == 1, "");
  static_assert(wdi.ok(), "");
  static_assert(wdi == weekday_indexed{sun, 1}, "");
  static_assert(wdi != weekday_indexed{sun, 2}, "");
  static_assert(wdi != weekday_indexed{mon, 1}, "");
  std::ostringstream os;
  os << wdi;
  assert(os.str() == "0[1]");
}

void test_weekday_last() {
  using namespace picidae::date;

  constexpr weekday_last wdl = sun[last];
  static_assert(wdl.weekday() == sun, "");
  static_assert(wdl.ok(), "");
  static_assert(wdl == weekday_last{sun}, "");
  static_assert(wdl != weekday_last{mon}, "");
  std::ostringstream os;
  os << wdl;
  assert(os.str() == "0[last]");
}

int main(int argc, char **argv) {
  test();
  test_month();
  test_year();
  test_weekday();
  test_weekday_indexed();
  test_weekday_last();

  return picidae::lightweighttest::report_errors();
}