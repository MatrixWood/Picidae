#ifndef PICIDAE_META_WHEN_HPP__
#define PICIDAE_META_WHEN_HPP__

#include "picidae/meta/common.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(meta)

template <bool B>
struct when;

template <typename... Dummy>
using when_valid = when<always_true_v<Dummy...>>;

PICIDAE_NAMESPACE_END(meta)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_WHEN_HPP__
