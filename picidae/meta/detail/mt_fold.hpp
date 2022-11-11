#ifndef PICIDAE_DETAIL_MT_FOLD_HPP__
#define PICIDAE_DETAIL_MT_FOLD_HPP__

namespace picidae {

namespace meta {

namespace detail {

template <class L, class V, template <class...> class F>
struct mt_fold_impl {};

template <template <class...> class L, class... T, class V,
          template <class...> class F>
struct mt_fold_impl<L<T...>, V, F> {
  static_assert(sizeof...(T) == 0, "T... must be empty");
  using type = V;
};

template <template <class...> class L, class T1, class... T, class V,
          template <class...> class F>
struct mt_fold_impl<L<T1, T...>, V, F> {
  using type = typename mt_fold_impl<L<T...>, F<V, T1>, F>::type;
};

template <template <class...> class L, class T1, class T2, class T3, class T4,
          class T5, class T6, class T7, class T8, class T9, class T10,
          class... T, class V, template <class...> class F>
struct mt_fold_impl<L<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T...>, V, F> {
  using type = typename mt_fold_impl<
      L<T...>,
      F<F<F<F<F<F<F<F<F<F<V, T1>, T2>, T3>, T4>, T5>, T6>, T7>, T8>, T9>, T10>,
      F>::type;
};

}  // namespace detail

template <class L, class V, template <class...> class F>
using mt_fold = typename detail::mt_fold_impl<L, V, F>::type;

template <class L, class V, class Q>
using mt_fold_q = typename detail::mt_fold_impl<L, V, Q::template fn>;

}  // namespace meta

}  // namespace picidae

#endif  // PICIDAE_DETAIL_MT_FOLD_HPP__
