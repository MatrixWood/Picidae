#include "picidae/date/date.hpp"

#include <iostream>
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
}

int main(int argc, char **argv) {
  picidae::lightweighttest::report_errors();

  return 0;
}