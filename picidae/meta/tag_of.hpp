#ifndef PICIDAE_META_TAG_OF_HPP__
#define PICIDAE_META_TAG_OF_HPP__

#include "picidae/meta/macro.hpp"
#include "picidae/meta/when.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(meta)

template <typename T, typename = void>
struct tag_of;

template <typename T, typename>
struct tag_of : tag_of<T, when<true>> {};

PICIDAE_NAMESPACE_BEGIN(core_detail)

template <typename...>
struct is_valid {
  static constexpr bool value = true;
};

PICIDAE_NAMESPACE_END(core_detail)

template <typename T, bool Condition>
struct tag_of<T, when<Condition>> {
  using type = T;
};

template <typename T>
struct tag_of<T, when<core_detail::is_valid<typename T::picidae_tag>::value>> {
  using type = typename T::picidae_tag;
};

template <typename T>
struct tag_of<T const> : tag_of<T> {};
template <typename T>
struct tag_of<T volatile> : tag_of<T> {};
template <typename T>
struct tag_of<T const volatile> : tag_of<T> {};
template <typename T>
struct tag_of<T&> : tag_of<T> {};
template <typename T>
struct tag_of<T&&> : tag_of<T> {};

template <typename T>
using tag_of_t = typename tag_of<T>::type;

PICIDAE_NAMESPACE_END(meta)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_TAG_OF_HPP__
