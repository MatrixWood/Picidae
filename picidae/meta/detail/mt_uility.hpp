#ifndef PICIDAE_META_DETAIL_MT_UTILITY_HPP__
#define PICIDAE_META_DETAIL_MT_UTILITY_HPP__

#include "picidae/meta/detail/mt_fold.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(meta)

template <class T>
struct mt_identity {
  using type = T;
};

template <class T>
using mt_identity_t = typename mt_identity<T>::type;

template <class... T>
struct mt_inherit : T... {};

PICIDAE_NAMESPACE_BEGIN(detail)

template <bool B, class T, class... E>
struct mt_if_b_impl {};

template <class T, class... E>
struct mt_if_b_impl<true, T, E...> {
  using type = T;
};

template <class T, class E>
struct mt_if_b_impl<false, T, E> {
  using type = E;
};

PICIDAE_NAMESPACE_END(detail)

template <bool B, class T, class... E>
using mt_if_b = typename detail::mt_if_b_impl<B, T, E...>::type;

template <class C, class T, class... E>
using mt_if =
    typename detail::mt_if_b_impl<static_cast<bool>(C::value), T, E...>::type;

PICIDAE_NAMESPACE_END(meta)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_DETAIL_MT_UTILITY_HPP__