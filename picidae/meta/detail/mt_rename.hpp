#ifndef PICIDAE_META_DETAIL_MT_RENAME_HPP__
#define PICIDAE_META_DETAIL_MT_RENAME_HPP__

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(meta)

PICIDAE_NAMESPACE_BEGIN(detail)

template <class A, template <class...> class B>
struct mt_rename_impl {};

template <template <class...> class A, class... T, template<class...> class B>
struct mt_rename_impl<A<T...>, B> {
  using type = B<T...>;
};

PICIDAE_NAMESPACE_END(detail)

template <class A, template <class...> class B>
using mt_rename = typename detail::mt_rename_impl<A, B>::type;

template <template <class...> class F, class L>
using mt_apply = typename detail::mt_rename_impl<L, F>::type;

template <class Q, class L>
using mt_apply_q = typename detail::mt_rename_impl<L, Q::template fn>::type;

PICIDAE_NAMESPACE_END(meta)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_DETAIL_MT_RENAME_HPP__