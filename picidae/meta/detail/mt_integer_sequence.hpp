#ifndef PICIDAE_META_DETAIL_MT_INTEGER_SEQUENCE_HPP__
#define PICIDAE_META_DETAIL_MT_INTEGER_SEQUENCE_HPP__

#include <utility>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(meta)

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
  using type = index_index_sequence<S...>;
};
template <size_t... N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
#endif

PICIDAE_NAMESPACE_END(meta)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_DETAIL_MT_INTEGER_SEQUENCE_HPP__s