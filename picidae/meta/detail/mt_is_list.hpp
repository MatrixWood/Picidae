#ifndef PICIDAE_META_DETAIL_MT_IS_LIST_HPP__
#define PICIDAE_META_DETAIL_MT_IS_LIST_HPP__

#include "picidae/meta/detail/mt_integral.hpp"

namespace picidae {

namespace meta {

namespace detail {

template <class L>
struct mt_is_list_impl {
  using type = mt_false;
};

template <template <class...> class L, class... T>
struct mt_is_list_impl<L<T...>> {
  using type = mt_true;
};

}  // namespace detail

template <class L>
using mt_is_list = typename detail::mt_is_list_impl<L>::type;

}  // namespace meta

}  // namespace picidae

#endif  // PICIDAE_META_DETAIL_MT_IS_LIST_HPP__
