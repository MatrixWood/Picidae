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

int main(int argc, char **argv) {
  test();
  picidae::lightweighttest::report_errors();

  return 0;
}