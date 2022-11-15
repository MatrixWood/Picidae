#ifndef PICIDAE_META_DETAIL_MT_INTEGER_SEQUENCE_HPP__
#define PICIDAE_META_DETAIL_MT_INTEGER_SEQUENCE_HPP__

#include <utility>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(meta)

template <class T, T... I>
struct integer_sequence {};

PICIDAE_NAMESPACE_BEGIN(detail)

// iseq_if_c
template <bool C, class T, class E>
struct iseq_if_c_impl;

template <class T, class E>
struct iseq_if_c_impl<true, T, E> {
  using type = T;
};

template <class T, class E>
struct iseq_if_c_impl<false, T, E> {
  using type = E;
};

template <bool C, class T, class E>
using iseq_if_c = typename iseq_if_c_impl<C, T, E>::type;

// iseq_identity
template <class T>
struct iseq_identity {
  using type = T;
};

template <class S1, class S2>
struct append_integer_sequence;

template <class T, T... I, T... J>
struct append_integer_sequence<integer_sequence<T, I...>,
                               integer_sequence<T, J...>> {
  using type = integer_sequence<T, I..., (J + sizeof...(I))...>;
};

template <class T, T N>
struct make_integer_sequence_impl;

template <class T, T N>
struct make_integer_sequence_impl_ {
 private:
  static_assert(N >= 0, "make_integer_sequence<T, N>: N must not be negative");

  static T const M = N / 2;
  static T const R = N % 2;

  using S1 = typename make_integer_sequence_impl<T, M>::type;
  using S2 = typename append_integer_sequence<S1, S1>::type;
  using S3 = typename make_integer_sequence_impl<T, R>::type;
  using S4 = typename append_integer_sequence<S2, S3>::type;

 public:
  using type = S4;
};

template <class T, T N>
struct make_integer_sequence_impl
    : iseq_if_c<N == 0, iseq_identity<integer_sequence<T>>,
                iseq_if_c<N == 1, iseq_identity<integer_sequence<T, 0>>,
                          make_integer_sequence_impl_<T, N>>> {};

PICIDAE_NAMESPACE_END(detail)

// make_integer_sequence
template <class T, T N>
using make_integer_sequence =
    typename detail::make_integer_sequence_impl<T, N>::type;

// index_sequence_for
template <class... T>
using index_sequence_for = make_integer_sequence<std::size_t, sizeof...(T)>;

// make_integer_sequence
#if defined(PICIDAE_CPP14)
using std::index_sequence;
using std::make_index_sequence;
#else
template <size_t...>
struct index_sequence {};
template <size_t N, size_t... S>
struct make_index_sequence_impl : make_index_sequence_impl<N - 1, N - 1, S...> {
};
template <size_t... S>
struct make_index_sequence_impl<0, S...> {
  using type = index_sequence<S...>;
};
template <size_t... N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
#endif

PICIDAE_NAMESPACE_END(meta)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_DETAIL_MT_INTEGER_SEQUENCE_HPP__s