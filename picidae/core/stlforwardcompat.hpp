#ifndef PICIDAE_CORE_STLFORWARDCOMPAT_HPP__
#define PICIDAE_CORE_STLFORWARDCOMPAT_HPP__

#include <optional>
#include <type_traits>

#include "picidae/core/common.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(core)

//===----------------------------------------------------------------------===//
//     Features from C++20
//===----------------------------------------------------------------------===//

template <typename T>
struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <typename T>
using remove_cvref_t = typename core::remove_cvref<T>::type;

PICIDAE_NAMESPACE_END(core)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_CORE_STLFORWARDCOMPAT_HPP__