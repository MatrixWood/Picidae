#include "picidae/meta/tag_of.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

namespace meta = picidae::meta;

struct NestedDatatype;
struct Nested { struct picidae_tag; };

struct PredicatedDatatype;
struct Predicated { static constexpr bool predicate = true; };

namespace picidae::meta {
template <typename T>
struct tag_of<T, meta::when<T::predicate>> {
    using type = PredicatedDatatype;
};
}

int main(int argc, char **argv) {
  static_assert(std::is_same_v<meta::tag_of_t<Nested>, Nested::picidae_tag>, "");
  static_assert(std::is_same_v<meta::tag_of_t<NestedDatatype>, NestedDatatype>, "");
  static_assert(std::is_same_v<meta::tag_of_t<Predicated>, PredicatedDatatype>, "");
}
