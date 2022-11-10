#ifndef PICIDAE_COMMON_HPP__
#define PICIDAE_COMMON_HPP__

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace picidae {

template <typename T>
struct always_false : std::false_type {};

template <typename T>
inline constexpr bool always_false_v = always_false<T>::value;

template <typename T>
struct always_true : std::true_type {};

template <typename T>
inline constexpr bool always_true_v = always_true<T>::value;

template <class T>
struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <typename T>
constexpr T e = static_cast<T>(2.7182818284590452353);

template <typename T>
constexpr T pi = static_cast<T>(3.1415926535897932385);

template <auto N, typename T = picidae::remove_cvref_t<decltype(N)>>
using c_ = std::integral_constant<T, N>;

using c_0 = c_<0>;

using c_1 = c_<1>;

using c_2 = c_<2>;

using c_3 = c_<3>;

using c_4 = c_<4>;

using c_5 = c_<5>;

using c_6 = c_<6>;

using c_7 = c_<7>;

using c_8 = c_<8>;

using c_9 = c_<9>;

template <auto N>
using bool_ = std::bool_constant<N>;

template <size_t N>
using index_t = c_<N, size_t>;

template <size_t N>
constexpr index_t<N> index = {};

template <auto v>
using constant_t = c_<v, std::decay_t<decltype(v)>>;

template <auto v>
constexpr constant_t<v> constant = {};

template <typename... Args>
using tuple_t = std::tuple<Args...>;

template <auto... N>
using is = std::index_sequence<N...>;

template <typename T>
inline constexpr auto typev = T::value;

template <typename T>
using value_t = typename T::value_type;

template <typename T>
inline constexpr auto negav = std::negation_v<T>;

template <typename T, typename = std::void_t<>>
struct _typeof : std::false_type {
  using type = T;
};

template <typename T>
struct _typeof<T, std::void_t<typename T::type>> : std::true_type {
  using type = typename T::type;
};

template <typename T>
using typeof_t = typename _typeof<T>::type;

template <typename T>
inline constexpr auto typeof_v = typev<typeof_t<T>>;

template <bool B, typename T, typename U>
using type_if = typeof_t<std::conditional_t<B, T, U>>;

template <bool B, typename T, typename U>
inline constexpr auto type_if_v = typev<type_if<B, T, U>>;

template <bool B, typename T, typename U>
inline constexpr auto value_if = typev<std::conditional_t<B, T, U>>;

template <typename T, typename U, typename V>
using conditional_of = std::conditional_t<typev<T>, U, V>;

template <typename T, typename = std::void_t<>>
struct has_type : std::false_type {};

template <typename T>
struct has_type<T, std::void_t<typename T::type>> : std::true_type {};

template <typename T>
inline constexpr auto has_type_v = typev<has_type<T>>;

template <typename T, typename = std::void_t<>>
struct has_value_type : std::false_type {
  using value_type = int;
};

template <typename T>
struct has_value_type<T, std::void_t<value_t<T>>> : std::true_type {
  using value_type = value_t<T>;
};

template <typename T>
using has_value_type_t = value_t<has_value_type<T>>;

template <typename T>
inline constexpr auto has_value_type_v = typev<has_value_type<T>>;

template <typename T, typename = std::void_t<>>
struct has_new : std::false_type {};

template <typename T>
struct has_new<T, std::void_t<decltype(T::operator new(0))>> : std::true_type {
};

template <typename T>
inline constexpr auto has_new_v = typev<has_new<T>>;

template <typename T, typename = std::void_t<>>
struct has_delete : std::false_type {};

template <typename T>
struct has_delete<T, std::void_t<decltype(T::operator delete(nullptr))>>
    : std::true_type {};

template <typename T>
inline constexpr auto has_delete_v = typev<has_delete<T>>;

template <typename T, typename = std::void_t<>>
struct is_type_complete : std::false_type {};

template <typename T>
struct is_type_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {
};
}  // namespace picidae

#endif  // PICIDAE_COMMON_HPP__
