#ifndef PICIDAE_CORE_NONNULL_HPP__
#define PICIDAE_CORE_NONNULL_HPP__

#include <type_traits>

#include "picidae/core/common.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(core)

template <typename T,
          typename std::enable_if_t<std::is_pointer_v<T>>* = nullptr>
using Nonnull = T _Nonnull;

PICIDAE_NAMESPACE_END(core)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_CORE_NONNULL_HPP__