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
  // std::cout << oss.str() << std::endl;
  PICIDAE_TEST(oss.str() == "01");

  d += days{11};
  oss.str("");
  oss << d;
  // std::cout << oss.str() << std::endl;
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
  // std::cout << unsigned(plus_m_ms) << std::endl;  // 10
  // std::cout << minus_m_m.count() << std::endl;    // 12
  // std::cout << unsigned(month_13) << std::endl;   // 1
  // std::cout << unsigned(month_12) << std::endl;   // 12
  // std::cout << plus_m_m.count() << std::endl;     // 2

  // std::cout << plus_m_ms << std::endl;  // 10
  // std::cout << month_13 << std::endl;   // 1
  // std::cout << month_12 << std::endl;   // 12

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
  // std::cout << os.str() << std::endl;

  // std::cout << (weekday{6} - weekday{1}).count() << std::endl;
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

void test_year_month() {
  using namespace picidae::date;

  constexpr year_month ym1 = {2015_y, jun};
  static_assert(ym1.year() == year{2015}, "");
  static_assert(ym1.month() == jun, "");
  static_assert(ym1.ok(), "");

  constexpr year_month ym2 = {2016_y, may};
  static_assert(ym2.year() == year{2016}, "");
  static_assert(ym2.month() == may, "");
  static_assert(ym2.ok(), "");

  static_assert(ym1 == ym1, "");
  static_assert(ym1 != ym2, "");
  static_assert(ym1 < ym2, "");
  static_assert(ym1 <= ym2, "");
  static_assert(ym2 > ym1, "");
  static_assert(ym2 >= ym2, "");

  static_assert(ym2 - ym1 == months{11}, "");
  static_assert(ym1 - ym2 == -months{11}, "");

  std::ostringstream os;
  os << ym1;
  assert(os.str() == "2015/6");

  static_assert(2015_y / aug == year_month{year{2015}, aug}, "");
  static_assert(2015_y / 8 == year_month{year{2015}, aug}, "");

  auto month_minus = 2015_y / 8_m - 2014_y / 3_m;
}

void test_month_day() {
  using namespace picidae::date;

  constexpr month_day md1 = {feb, day{28}};
  constexpr month_day md2 = {mar, day{1}};
  static_assert(md1.ok(), "");
  static_assert(md2.ok(), "");
  static_assert(!month_day{feb, day{32}}.ok(), "");
  static_assert(!month_day{month{0}, day{1}}.ok(), "");
  static_assert(md1.month() == feb, "");
  static_assert(md1.day() == day{28}, "");
  static_assert(md2.month() == mar, "");
  static_assert(md2.day() == day{1}, "");
  static_assert(md1 == md1, "");
  static_assert(md1 != md2, "");
  static_assert(md1 < md2, "");
  std::ostringstream os;
  os << md1;
  assert(os.str() == "2/28");

  static_assert(aug / 14_d == month_day{month{8}, day{14}}, "");
  static_assert(aug / 14 == month_day{month{8}, day{14}}, "");
  static_assert(8 / 14_d == month_day{month{8}, day{14}}, "");
  static_assert(14_d / aug == month_day{month{8}, day{14}}, "");
  static_assert(14_d / 8 == month_day{month{8}, day{14}}, "");
}

void test_month_weekday() {
  using namespace picidae::date;

  constexpr month_weekday mwd1 = {feb, sat[4]};
  constexpr month_weekday mwd2 = {mar, mon[1]};
  static_assert(mwd1.ok(), "");
  static_assert(mwd2.ok(), "");
  static_assert(!month_weekday{feb, sat[0]}.ok(), "");
  static_assert(!month_weekday{month{0}, sun[1]}.ok(), "");
  static_assert(mwd1.month() == feb, "");
  static_assert(mwd1.weekday_indexed() == sat[4], "");
  static_assert(mwd2.month() == mar, "");
  static_assert(mwd2.weekday_indexed() == mon[1], "");
  static_assert(mwd1 == mwd1, "");
  static_assert(mwd1 != mwd2, "");
  std::ostringstream os;
  os << mwd1;
  assert(os.str() == "2/6[4]");

  static_assert(
      aug / fri[2] == month_weekday{month{8}, weekday_indexed{weekday{5u}, 2}},
      "");
  static_assert(
      8 / fri[2] == month_weekday{month{8}, weekday_indexed{weekday{5u}, 2}},
      "");
  static_assert(
      fri[2] / aug == month_weekday{month{8}, weekday_indexed{weekday{5u}, 2}},
      "");
  static_assert(
      fri[2] / 8 == month_weekday{month{8}, weekday_indexed{weekday{5u}, 2}},
      "");
}

void test_month_day_last() {
  using namespace picidae::date;

  constexpr month_day_last mdl1{feb};
  constexpr month_day_last mdl2{mar};
  static_assert(mdl1.ok(), "");
  static_assert(mdl2.ok(), "");
  static_assert(!month_day_last{month{0}}.ok(), "");
  static_assert(mdl1.month() == feb, "");
  static_assert(mdl2.month() == mar, "");
  static_assert(mdl1 == mdl1, "");
  static_assert(mdl1 != mdl2, "");
  static_assert(mdl1 < mdl2, "");
  std::ostringstream os;
  os << mdl1;
  assert(os.str() == "2/last");

  static_assert(aug / last == month_day_last{month{8}}, "");
  static_assert(8 / last == month_day_last{month{8}}, "");
  static_assert(last / aug == month_day_last{month{8}}, "");
  static_assert(last / 8 == month_day_last{month{8}}, "");
}

void test_year_month_day() {
  using namespace picidae::date;

  constexpr year_month_day ymd1 = {2015_y, aug, 9_d};
  static_assert(ymd1.ok(), "");
  static_assert(ymd1.year() == 2015_y, "");
  static_assert(ymd1.month() == aug, "");
  static_assert(ymd1.day() == 9_d, "");
  std::ostringstream os;
  os << ymd1;
  assert(os.str() == "2015-08-09");

  static_assert((2000_y / feb / 29).ok(), "");
  static_assert(!(2000_y / feb / 30).ok(), "");
  static_assert((2100_y / feb / 28).ok(), "");
  static_assert(!(2100_y / feb / 29).ok(), "");

  PICIDAE_TEST(sys_days(2100_y / feb / 28) + days{1} ==
               sys_days(2100_y / mar / 1));
  PICIDAE_TEST(sys_days(2000_y / mar / 1) - sys_days(2000_y / feb / 28) ==
               days{2});
  PICIDAE_TEST(sys_days(2100_y / mar / 1) - sys_days(2100_y / feb / 28) ==
               days{1});

  PICIDAE_TEST_EQ(jan / 31 / 2015, jan / last / 2015);
  PICIDAE_TEST_EQ(feb / 28 / 2015, feb / last / 2015);
  PICIDAE_TEST_EQ(mar / 31 / 2015, mar / last / 2015);
  PICIDAE_TEST_EQ(apr / 30 / 2015, apr / last / 2015);
  PICIDAE_TEST_EQ(may / 31 / 2015, may / last / 2015);
  PICIDAE_TEST_EQ(jun / 30 / 2015, jun / last / 2015);
  PICIDAE_TEST_EQ(jul / 31 / 2015, jul / last / 2015);
  PICIDAE_TEST_EQ(aug / 31 / 2015, aug / last / 2015);
  PICIDAE_TEST_EQ(sep / 30 / 2015, sep / last / 2015);
  PICIDAE_TEST_EQ(oct / 31 / 2015, oct / last / 2015);
  PICIDAE_TEST_EQ(nov / 30 / 2015, nov / last / 2015);
  PICIDAE_TEST_EQ(dec / 31 / 2015, dec / last / 2015);

  PICIDAE_TEST_EQ(jan / 31 / 2016, jan / last / 2016);
  PICIDAE_TEST_EQ(feb / 29 / 2016, feb / last / 2016);
  PICIDAE_TEST_EQ(mar / 31 / 2016, mar / last / 2016);
  PICIDAE_TEST_EQ(apr / 30 / 2016, apr / last / 2016);
  PICIDAE_TEST_EQ(may / 31 / 2016, may / last / 2016);
  PICIDAE_TEST_EQ(jun / 30 / 2016, jun / last / 2016);
  PICIDAE_TEST_EQ(jul / 31 / 2016, jul / last / 2016);
  PICIDAE_TEST_EQ(aug / 31 / 2016, aug / last / 2016);
  PICIDAE_TEST_EQ(sep / 30 / 2016, sep / last / 2016);
  PICIDAE_TEST_EQ(oct / 31 / 2016, oct / last / 2016);
  PICIDAE_TEST_EQ(nov / 30 / 2016, nov / last / 2016);
  PICIDAE_TEST_EQ(dec / 31 / 2016, dec / last / 2016);

  static_assert(
      2015_y / aug / 14_d == year_month_day{year{2015}, month{8}, day{14}}, "");
  static_assert(
      2015_y / aug / 14 == year_month_day{year{2015}, month{8}, day{14}}, "");
  static_assert(
      2015_y / (aug / 14_d) == year_month_day{year{2015}, month{8}, day{14}},
      "");
  static_assert(
      2015 / (aug / 14_d) == year_month_day{year{2015}, month{8}, day{14}}, "");
  static_assert(
      aug / 14_d / 2015_y == year_month_day{year{2015}, month{8}, day{14}}, "");
  static_assert(
      aug / 14_d / 2015 == year_month_day{year{2015}, month{8}, day{14}}, "");
}

void test_year_month_day_last() {
  using namespace picidae::date;

  constexpr year_month_day_last ymdl1 = {2015_y, month_day_last{aug}};
  static_assert(ymdl1.ok(), "");
  static_assert(ymdl1.year() == 2015_y, "");
  static_assert(ymdl1.month() == aug, "");
  static_assert(ymdl1.month_day_last() == month_day_last{aug}, "");
  static_assert(ymdl1.day() == 31_d, "");

  constexpr year_month_day_last ymdl2 = {2015_y, month_day_last{sep}};

  static_assert(ymdl1 == ymdl1, "");
  static_assert(ymdl1 != ymdl2, "");
  static_assert(ymdl1 < ymdl2, "");
  static_assert(ymdl2 > ymdl1, "");
  static_assert(ymdl1 <= ymdl1, "");
  static_assert(ymdl2 >= ymdl1, "");

  std::ostringstream os;
  os << ymdl1;
  assert(os.str() == "2015/8/last");

  static_assert(2015_y / aug / last ==
                    year_month_day_last{year{2015}, month_day_last{month{8}}},
                "");
  static_assert(2015_y / (aug / last) ==
                    year_month_day_last{year{2015}, month_day_last{month{8}}},
                "");
  static_assert(2015 / (aug / last) ==
                    year_month_day_last{year{2015}, month_day_last{month{8}}},
                "");
  static_assert(aug / last / 2015_y ==
                    year_month_day_last{year{2015}, month_day_last{month{8}}},
                "");
  static_assert(aug / last / 2015 ==
                    year_month_day_last{year{2015}, month_day_last{month{8}}},
                "");
}

int main(int argc, char **argv) {
  test();
  test_month();
  test_year();
  test_weekday();
  test_weekday_indexed();
  test_weekday_last();
  test_year_month();
  test_month_day();
  test_month_day_last();
  test_month_weekday();
  test_year_month_day();
  test_year_month_day_last();

  return picidae::lightweighttest::report_errors();
}