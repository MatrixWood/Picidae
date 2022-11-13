#include <iostream>

#include "picidae/meta/common.hpp"
#include "picidae/meta/macro.hpp"

using namespace picidae;

enum AnimalFlags { HasClaws = 1, CanFly = 2, EatsFish = 4, Endangered = 8 };

inline AnimalFlags operator|(AnimalFlags a, AnimalFlags b) {
  return static_cast<AnimalFlags>(
      static_cast<std::underlying_type_t<AnimalFlags>>(a) |
      static_cast<std::underlying_type_t<AnimalFlags>>(b));
}

PICIDAE_ENUM_FLAGS(Hands){NoHands = 0, OneHand = 1 << 0, TwoHands = 1 << 1,
                          LeftHand = 1 << 2, RightHand = 1 << 3};

void test_enum_flag_operator() {
  {
    AnimalFlags flags = CanFly | EatsFish | Endangered;
    std::cout << flags << std::endl;

    constexpr bool is_animal_flag =
        std::is_same_v<std::underlying_type_t<AnimalFlags>, int>;
    std::cout << is_animal_flag << std::endl;
  }

  {
    Hands hands = Hands::OneHand | Hands::TwoHands;
    if (((hands & ~Hands::OneHand) ^ (Hands::TwoHands)) == Hands::NoHands) {
      std::cout << "no hands!" << std::endl;
    }
  }
}

int main() { test_enum_flag_operator(); }