#ifndef PICIDAE_META_DETAIL_MT_VOID_HPP__
#define PICIDAE_META_DETAIL_MT_VOID_HPP__

namespace picidae {

namespace meta {

namespace detail {

template <class... T>
struct mt_void_impl {
  using type = void;
};
}  // namespace detail

template <class... T>
using mt_void = typename detail::mt_void_impl<T...>::type;

}  // namespace meta

}  // namespace picidae

#endif  // PICIDAE_META_DETAIL_MT_VOID_HPP__
