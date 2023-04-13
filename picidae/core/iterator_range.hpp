#ifndef PICIDAE_CORE_ITERATOR_RANGE_HPP__
#define PICIDAE_CORE_ITERATOR_RANGE_HPP__

#include <type_traits>
#include <utility>

#include "picidae/core/common.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(core)

PICIDAE_NAMESPACE_BEGIN(detail)

template <typename T, typename _ = void>
struct has_begin_end : std::false_type {};

template <typename T>
struct has_begin_end<T, std::void_t<decltype(std::declval<T>().begin()),
                                    decltype(std::declval<T>().end())>>
    : public std::true_type {};

PICIDAE_NAMESPACE_END(detail)

template <typename IteratorT>
class iterator_range {
  IteratorT beg_it, end_it;

 public:
  template <typename Container,
            typename = std::enable_if_t<detail::has_begin_end<Container>::value>>
  iterator_range(Container &&c) : beg_it(c.begin()), end_it(c.end()) {}

  iterator_range(IteratorT beg_it, IteratorT end_it)
      : beg_it(std::move(beg_it)), end_it(std::move(end_it)) {}

  IteratorT begin() const { return beg_it; }

  IteratorT end() const { return end_it; }

  bool empty() const { return beg_it == end_it; }
};

template <class T>
iterator_range<T> make_range(T x, T y) {
  return iterator_range<T>(std::move(x), std::move(y));
}

template <class T>
iterator_range<T> make_range(std::pair<T, T> p) {
  return iterator_range<T>(std::move(p.first), std::move(p.second));
}

PICIDAE_NAMESPACE_END(core)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_CORE_ITERATOR_RANGE_HPP__