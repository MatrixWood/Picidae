#ifndef PICIDAE_COMMON_HPP__
#define PICIDAE_COMMON_HPP__

#include <type_traits>

namespace picidae {

template <typename T>
struct always_false : std::false_type {};

template <typename T>
inline constexpr bool always_false_v = always_false<T>::value;

template <typename T>
struct always_true : std::true_type {};

template <typename T>
inline constexpr bool always_true_v = always_true<T>::value;

template< class T >
struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template< class T >
using remove_cvref_t = typename remove_cvref<T>::type;


} // namespace picidae

#endif // PICIDAE_COMMON_HPP__
