#include "picidae/meta/default.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

namespace meta = picidae::meta;

template <typename T>
struct method_impl : meta::default_ {};

template <>
struct method_impl<int> {};

static_assert(meta::is_default<method_impl<void>>{}, "");
static_assert(!meta::is_default<method_impl<int>>{}, "");

int main(int argc, char **argv) {}