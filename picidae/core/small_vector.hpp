#ifndef PICIDAE_CORE_SMALL_VECTOR_HPP__
#define PICIDAE_CORE_SMALL_VECTOR_HPP__

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <type_traits>

#include "picidae/core/arrayref.hpp"
#include "picidae/core/iterator_range.hpp"
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
class ArrayRef;

template <typename IteratorT>
class iterator_range;

template <typename T>
struct IsPod : std::integral_constant<bool, std::is_standard_layout<T>::value &&
                                                std::is_trivial<T>::value> {};

template <class Size_T>
class SmallVectorBase {
 protected:
  void *BeginX;
  Size_T Size = 0, Capacity;

  static constexpr size_t SizeTypeMax() {
    return std::numeric_limits<Size_T>::max();
  }

  SmallVectorBase() = delete;
  SmallVectorBase(void *FirstEl, size_t TotalCapacity)
      : BeginX(FirstEl), Capacity(TotalCapacity) {}

  void* mallocForGrow(void *FirstEl, size_t MinSize, size_t TSize,
                      size_t& NewCapacity);
  
  void grow_pod(void *FirstEl, size_t MinSize, size_t TSize);

  void *replaceAllocation(void *NewElts, size_t TSize, size_t NewCapacity,
                          size_t VSize = 0);

 public:
  size_t size() const { return Size; }
  size_t capacity() const { return Capacity; }
  [[nodiscard]] bool empty() const { return !Size; }

 protected:
  void set_size(size_t N) {
    assert(N <= capacity());
    Size = N;
  }
};

template <class T>
using SmallVectorSizeType =
    std::conditional_t<sizeof(T) < 4 && sizeof(void *) >= 8, uint64_t,
                       uint32_t>;

// TODO
template <class T, typename = void>
struct SmallVectorAlignmentAndSize {
  alignas(SmallVectorBase<SmallVectorSizeType<T>>)
  char Base[sizeof(SmallVectorBase<SmallVectorSizeType<T>>)];
  
};

template <typename T, typename = void>
class SmallVectorTemplateCommon
    : public SmallVectorBase<SmallVectorSizeType<T>> {
  using Base = SmallVectorBase<SmallVectorSizeType<T>>;

 protected:
  void* getFirstEl() const {
    return const_cast<void *>(reinterpret_cast<const void*>(

    ))
  }
};

PICIDAE_NAMESPACE_END(core)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_CORE_SMALL_VECTOR_HPP__