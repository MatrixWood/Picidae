#ifndef PICIDAE_CORE_SMALL_VECTOR_HPP__
#define PICIDAE_CORE_SMALL_VECTOR_HPP__

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iterator>
#include <memory>

#include "picidae/meta/macro.hpp"

#if defined(__GNUC__)
#define TF_LIKELY(x) (__builtin_expect((x), 1))
#define TF_UNLIKELY(x) (__builtin_expect((x), 0))
#else
#define TF_LIKELY(x) (x)
#define TF_UNLIKELY(x) (x)
#endif

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(core)

PICIDAE_NAMESPACE_BEGIN(detail)

inline uint64_t next_capacity(uint64_t A) {
  A |= (A >> 1);
  A |= (A >> 2);
  A |= (A >> 4);
  A |= (A >> 8);
  A |= (A >> 16);
  A |= (A >> 32);
  return A + 1;
}

PICIDAE_NAMESPACE_END(detail)

template <typename T>
struct IsPod : std::integral_constant<bool, std::is_standard_layout<T>::value &&
                                                std::is_trivial<T>::value> {};

class SmallVectorBase {
 protected:
  void* _beg;
  void* _end;
  void* _capacity;

 protected:
  SmallVectorBase(void* first_elem, size_t size)
      : _beg(first_elem),
        _end(first_elem),
        _capacity((char*)first_elem + size) {}

 public:
  size_t size_in_bytes() const {
    return size_t((char*)_end - (char*)_beg);
  }

  size_t capacity_in_bytes() const {
    return size_t((char*)_capacity - (char*)_beg);
  }

  bool empty() const {
    return _beg == _end;
  }
};



PICIDAE_NAMESPACE_END(core)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_CORE_SMALL_VECTOR_HPP__