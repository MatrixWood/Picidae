#include "picidae/meta/when.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

namespace meta = picidae::meta;

template <typename T, typename = meta::when<true>>
struct base_template;

template <typename T>
struct base_template<T, meta::when_valid<typename T::first_type>> {};

template <typename T>
struct base_template<T, meta::when_valid<typename T::second_type>> {};

struct First {
  struct first_type;
};
struct Second {
  struct second_type;
};

template struct base_template<First>;
template struct base_template<Second>;

int main(int argc, char **argv) {}
