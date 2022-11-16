#ifndef PICIDAE_META_DEFAULT_HPP__
#define PICIDAE_META_DEFAULT_HPP__

#include "picidae/meta/common.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(meta)

struct default_ {};

template <typename T, typename = void>
struct is_default;

template <typename Method, typename>
struct is_default : std::false_type {};

template <typename Method>
struct is_default<Method, decltype((void)static_cast<default_>(*(Method*)0))>
    : std::true_type {};

PICIDAE_NAMESPACE_END(meta)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_DEFAULT_HPP__