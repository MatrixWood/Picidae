#ifndef PICIDAE_META_DETAIL_MT_MIN_ELEMENT_HPP__
#define PICIDAE_META_DETAIL_MT_MIN_ELEMENT_HPP__

#include "picidae/meta/detail/mt_fold.hpp"
#include "picidae/meta/detail/mt_uility.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(meta)

PICIDAE_NAMESPACE_BEGIN(detail)

template <template <class...> class P>
struct select_min {
  template <class T1, class T2>
  using fn = mt_if<P<T1, T2>, T1, T2>;
};

PICIDAE_NAMESPACE_END(detail)



PICIDAE_NAMESPACE_END(meta)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_DETAIL_MT_MIN_ELEMENT_HPP__
