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


} // namespace picidae

#endif // PICIDAE_COMMON_HPP__
