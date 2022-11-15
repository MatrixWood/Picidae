#ifndef PICIDAE_META_COMMON_HPP__
#define PICIDAE_META_COMMON_HPP__

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include "picidae/meta/macro.hpp"

namespace picidae {

namespace meta {

template <typename...>
struct always_false : std::false_type {};

template <typename... Ts>
inline constexpr bool always_false_v = always_false<Ts...>::value;

template <typename ...>
struct always_true : std::true_type {};

template <typename... Ts>
inline constexpr bool always_true_v = always_true<Ts...>::value;

template <bool B>
struct when;

template <typename... Dummy>
using when_valid = when<always_true_v<Dummy...>>;

template <class T>
struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

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

// todo: need to check for c++ standard support
/*

template <typename T>
constexpr T e = static_cast<T>(2.7182818284590452353);

template <typename T>
constexpr T pi = static_cast<T>(3.1415926535897932385);

template <auto N, typename T = picidae::meta::remove_cvref_t<decltype(N)>>
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

template <typename T>
inline constexpr auto is_type_complete_v = typev<is_type_complete<T>>;

#if __cplusplus >= 202002L

template <template <typename...> typename B, typename T,
          typename = std::void_t<>>
struct is_base_template_of : std::false_type {};

template <template <typename...> typename B, typename T>
struct is_base_template_of<
    B, T, std::void_t<decltype([]<typename... Args>(B<Args...>*) {
    }(std::declval<T*>()))>> : std::true_type {};

template <template <typename...> typename B, typename T>
inline constexpr auto is_base_template_of_v = typev<is_base_template_of<B, T>>;

#endif

template <typename T>
constexpr bool is_template() {
  return false;
}

template <template <auto...> typename T>
constexpr bool is_template() {
  return true;
}

template <template <typename...> typename T>
constexpr bool is_template() {
  return true;
}

template <template <typename, auto...> typename T>
constexpr bool is_template() {
  return true;
}

template <template <template <auto...> typename...> typename T>
constexpr bool is_template() {
  return true;
}

template <template <template <typename...> typename...> typename T>
constexpr bool is_template() {
  return true;
}

template <template <template <typename, auto...> typename...> typename T>
constexpr bool is_template() {
  return true;
}

template <typename T, typename U>
struct pair_t {
  using first = T;
  using second = U;
};

template <typename T>
using first_t = typename T::first;

template <typename T>
using second_t = typename T::second;

template <int p, int q>
struct pair_v {
  static constexpr auto first = p;
  static constexpr auto second = q;
};

template <typename T>
inline constexpr auto first_v = T::first;

template <typename T>
inline constexpr auto second_v = T::second;

template <typename T>
inline constexpr auto pair_diff = second_v<T> - first_v<T>;

template <auto p, auto q, typename T>
struct triple : std::__type_identity<T> {
  static constexpr auto first = p;
  static constexpr auto second = q;
};

template <int N, typename T>
struct identity {
  using type = T;
};

template <int N, typename T>
using identity_t = typeof_t<identity<N, T>>;

template <auto N, typename T>
constexpr decltype(auto) ignore(T&& t) {
  return std::forward<T>(t);
}

template <auto N, typename T>
struct wrapper : wrapper<N - 1, std::__type_identity<T>> {};

template <typename T>
struct wrapper<0, T> : std::__type_identity<T> {};

template <auto N, typename T>
using wrapper_t = typeof_t<wrapper<N, T>>;

template <auto N, typename T>
struct index_type : std::__type_identity<T> {
  static constexpr auto value = N;
};

template <auto N, typename T>
using index_upper = wrapper_t<1, index_type<N, T>>;

template <typename T, typename...>
struct alias {
  using type = T;
};

template <typename... Args>
using alias_t = typeof_t<alias<Args...>>;

template <template <typename...> typename F, typename... Args>
struct wrapin {
  using type = tuple_t<F<Args>...>;
};

template <template <typename...> typename F, typename... Args>
using wrapin_t = typeof_t<wrapin<F, Args...>>;

template <bool B, template <typename...> typename F, typename T>
using wrapin_if = std::conditional_t<B, F<T>, T>;

template <typename... Args>
struct contains : std::false_type {};

template <typename T, typename... Args>
struct contains<T, Args...> : bool_<(std::is_same_v<T, Args> || ...)> {};

template <typename...>
inline constexpr auto contains_v = std::false_type{};

template <typename T, typename... Args>
inline constexpr auto contains_v<T, Args...> = (std::is_same_v<T, Args> || ...);

template <auto... values>
struct comprise : std::false_type {};

template <auto value, auto... values>
struct comprise<value, values...> : bool_<((value == values) || ...)> {};

template <auto...>
inline constexpr auto comprise_v = std::false_type{};

template <auto value, auto... values>
inline constexpr auto comprise_v<value, values...> = ((value == values) || ...);

template <typename B, typename...>
struct exists_type : B {};

template <typename B, typename T, typename... Args>
struct exists_type<B, T, Args...>
    : std::conditional_t<contains_v<T, Args...>, std::negation<B>,
                         exists_type<B, Args...>> {};

template <typename B, typename... Args>
using exists_type_t = typeof_t<exists_type<B, Args...>>;

template <typename B, typename... Args>
inline constexpr auto exists_type_v = typev<exists_type_t<B, Args...>>;

template <typename... Args>
using is_unique_type = exists_type<std::true_type, Args...>;

template <typename...>
inline constexpr auto is_unique_type_v = std::true_type{};

template <typename T, typename... Args>
inline constexpr auto is_unique_type_v<T, Args...> =
    !contains_v<T, Args...> && is_unique_type_v<Args...>;

template <typename... Args>
using has_duplicates_type = exists_type<std::false_type, Args...>;

template <typename...>
inline constexpr auto has_duplicates_type_v = std::false_type{};

template <typename T, typename... Args>
inline constexpr auto has_duplicates_type_v<T, Args...> =
    contains_v<T, Args...> || has_duplicates_type_v<Args...>;

template <typename B, auto...>
struct exists_value : B {};

template <typename B, auto value, auto... values>
struct exists_value<B, value, values...>
    : std::conditional_t<comprise_v<value, values...>, std::negation<B>,
                         exists_value<B, values...>> {};

template <typename B, auto... values>
using exists_value_t = typeof_t<exists_value<B, values...>>;

template <typename B, auto... values>
inline constexpr auto exists_value_v = typev<exists_value_t<B, values...>>;

template <auto... values>
using is_unique_value = exists_value<std::true_type, values...>;

template <auto...>
inline constexpr auto is_unique_value_v = std::true_type{};

template <auto value, auto... values>
inline constexpr auto is_unique_value_v<value, values...> =
    negav<comprise<value, values...>>&& is_unique_value_v<values...>;

template <auto... values>
using has_duplicates_value = exists_value<std::false_type, values...>;

template <auto...>
inline constexpr auto has_duplicates_value_v = std::false_type{};

template <auto value, auto... values>
inline constexpr auto has_duplicates_value_v<value, values...> =
    typev<comprise<value, values...>> || has_duplicates_value_v<values...>;

template <bool B, typename T>
struct exists;

template <template <typename...> typename T, typename... Args>
struct exists<true, T<Args...>> {
  using type = is_unique_type<Args...>;
};

template <template <typename, auto...> typename T, typename U, auto... values>
struct exists<true, T<U, values...>> {
  using type = is_unique_value<values...>;
};

template <template <typename...> typename T, typename... Args>
struct exists<false, T<Args...>> {
  using type = has_duplicates_type<Args...>;
};

template <template <typename, auto...> typename T, typename U, auto... values>
struct exists<false, T<U, values...>> {
  using type = has_duplicates_value<values...>;
};

template <bool B, typename T>
using exists_t = typeof_t<exists<B, T>>;

template <bool B, typename T>
inline constexpr auto exists_v = typev<exists_t<B, T>>;

template <typename T>
using is_unique = exists<true, T>;

template <typename T>
using is_unique_t = typeof_t<is_unique<T>>;

template <typename T>
inline constexpr auto is_unique_v = typev<is_unique_t<T>>;

template <typename T>
using has_duplicates = exists<false, T>;

template <typename T>
using has_duplicates_t = typeof_t<has_duplicates<T>>;

template <typename T>
inline constexpr auto has_duplicates_v = typev<has_duplicates_t<T>>;

template <bool A, bool B, typename X, typename Y, typename Z>
using ternary_conditional = std::conditional<A, std::conditional_t<B, X, Y>, Z>;

template <bool A, bool B, typename X, typename Y, typename Z>
using ternary_conditional_t = typeof_t<ternary_conditional<A, B, X, Y, Z>>;

template <typename... Args>
struct inherit : Args... {};

template <typename T, typename U = std::void_t<>>
struct is_inheritable : std::false_type {};

template <typename T>
struct is_inheritable<T, std::void_t<inherit<T>>> : std::true_type {};

template <typename T>
inline constexpr auto is_inheritable_v = typev<is_inheritable<T>>;

template <typename... Args>
struct is_inheritable_pack : bool_<(is_inheritable_v<Args> && ...)> {};

template <typename... Args>
inline constexpr auto is_inheritable_pack_v =
    typev<is_inheritable_pack<Args...>>;

template <typename T>
struct is_instantiable : std::negation<std::is_abstract<T>> {};

template <typename T>
inline constexpr auto is_instantiable_v = typev<is_instantiable<T>>;

template <bool B, typename T = std::void_t<>>
struct disable_if {};

template <typename T>
struct disable_if<false, T> : std::__type_identity<T> {};

template <bool B, typename T = std::void_t<>>
using disable_if_t = typeof_t<disable_if<B, T>>;

template <typename T, typename U>
struct transfer_reference : std::__type_identity<U> {};

template <typename T, typename U>
struct transfer_reference<T&, U> : std::add_lvalue_reference<U> {};

template <typename T, typename U>
struct transfer_reference<T&&, U> : std::add_rvalue_reference<U> {};

template <typename T, typename U>
using transfer_reference_t = typeof_t<transfer_reference<T, U>>;

template <typename T, typename U>
struct _transfer_cv : std::__type_identity<U> {};

template <typename T, typename U>
struct _transfer_cv<const T, U>
    : transfer_reference<U, std::add_const_t<std::remove_reference_t<U>>> {};

template <typename T, typename U>
struct _transfer_cv<volatile T, U>
    : transfer_reference<U, std::add_volatile_t<std::remove_reference_t<U>>> {};

template <typename T, typename U>
struct _transfer_cv<const volatile T, U>
    : transfer_reference<U, std::add_cv_t<std::remove_reference_t<U>>> {};

template <typename T, typename U>
struct transfer_cv : _transfer_cv<std::remove_reference_t<T>, U> {};

template <typename T, typename U>
using transfer_cv_t = typeof_t<transfer_cv<T, U>>;

template <typename T, typename U>
struct transfer_cvref : transfer_reference<T, transfer_cv_t<T, U>> {};

template <typename T, typename U>
using transfer_cvref_t = typeof_t<transfer_cvref<T, U>>;

template <typename S, typename T, typename = std::void_t<>>
struct is_streamable : std::false_type {};

template <typename S, typename T>
struct is_streamable<
    S, T,
    std::void_t<disable_if_t<std::is_same_v<S, T>>,
                decltype(std::declval<std::add_lvalue_reference_t<S>>()
                         << std::declval<T>())>> : std::true_type {};

template <typename S, typename T>
using is_streamable_t = typeof_t<is_streamable<S, T>>;

template <typename S, typename T>
inline constexpr auto is_streamable_v = typev<is_streamable_t<S, T>>;

template <typename T, typename = std::void_t<>>
struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T>
inline constexpr auto is_iterable_v = typev<is_iterable<T>>;

template <typename T, typename = std::void_t<>>
struct is_container : std::false_type {};

template <typename T>
struct is_container<
    T,
    std::void_t<typename T::value_type, typename T::size_type,
                typename T::allocator_type, typename T::iterator,
                typename T::const_iterator, decltype(std::declval<T>().size()),
                decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end()),
                decltype(std::declval<T>().cbegin()),
                decltype(std::declval<T>().cend())>> : std::true_type {};

template <typename T>
inline constexpr auto is_container_v = typev<is_container<T>>;

template <typename T, typename U>
struct is_pointer_of : std::is_same<T, std::add_pointer_t<U>> {};

template <typename T, typename U>
inline constexpr auto is_pointer_of_v = typev<is_pointer_of<T, U>>;

template <template <typename...> typename T, typename Args>
struct is_instance_of : std::false_type {};

template <template <typename...> typename T, typename... Args>
struct is_instance_of<T, T<Args...>> : std::true_type {};

template <template <typename...> typename T, typename Args>
inline constexpr auto is_instance_of_v = typev<is_instance_of<T, Args>>;

template <template <typename, auto...> typename T, typename Args>
struct is_sequence_of : std::false_type {};

template <template <typename, auto...> typename T, typename U, auto... values>
struct is_sequence_of<T, T<U, values...>> : std::true_type {};

template <template <typename, auto...> typename T, typename Args>
inline constexpr auto is_sequence_of_v = typev<is_sequence_of<T, Args>>;

template <typename T>
struct is_tuple : is_instance_of<std::tuple, T> {};

template <typename T>
inline constexpr auto is_tuple_v = typev<is_tuple<T>>;

template <typename T>
struct is_sequence : is_sequence_of<std::integer_sequence, T> {};

template <typename T>
inline constexpr auto is_sequence_v = typev<is_sequence<T>>;

template <typename T>
struct is_variadic_type : std::false_type {};

template <template <typename...> typename T, typename... Args>
struct is_variadic_type<T<Args...>> : std::true_type {};

template <typename T>
using is_variadic_type_t = typeof_t<is_variadic_type<T>>;

template <typename T>
inline constexpr auto is_variadic_type_v = typev<is_variadic_type_t<T>>;

template <typename T>
struct is_variadic_value : std::false_type {};

template <template <auto...> typename T, auto... Args>
struct is_variadic_value<T<Args...>> : std::true_type {};

template <template <typename, auto...> typename T, typename U, auto... Args>
struct is_variadic_value<T<U, Args...>> : std::true_type {};

template <typename T>
using is_variadic_value_t = typeof_t<is_variadic_value<T>>;

template <typename T>
inline constexpr auto is_variadic_value_v = typev<is_variadic_value_t<T>>;

template <typename T>
struct is_variadic : bool_<is_variadic_type_v<T> || is_variadic_value_v<T>> {};

template <typename T>
using is_variadic_t = typeof_t<is_variadic<T>>;

template <typename T>
inline constexpr auto is_variadic_v = typev<is_variadic_t<T>>;

template <typename... Args>
struct is_variadic_type_pack : bool_<(is_variadic_type_v<Args> && ...)> {};

template <typename... Args>
using is_variadic_type_pack_t = typeof_t<is_variadic_type_pack<Args...>>;

template <typename... Args>
inline constexpr auto is_variadic_type_pack_v =
    typev<is_variadic_type_pack_t<Args...>>;

template <typename... Args>
struct is_variadic_value_pack : bool_<(is_variadic_value_v<Args> && ...)> {};

template <typename... Args>
using is_variadic_value_pack_t = typeof_t<is_variadic_value_pack<Args...>>;

template <typename... Args>
inline constexpr auto is_variadic_value_pack_v =
    typev<is_variadic_value_pack_t<Args...>>;

template <typename... Args>
struct is_variadic_pack : bool_<is_variadic_type_pack_v<Args...> ||
                                is_variadic_value_pack_v<Args...>> {};

template <typename... Args>
using is_variadic_pack_t = typeof_t<is_variadic_pack<Args...>>;

template <typename... Args>
inline constexpr auto is_variadic_pack_v = typev<is_variadic_pack_t<Args...>>;

template <typename T>
struct is_group : std::false_type {};

template <template <typename...> typename T, typename... Args>
struct is_group<T<Args...>> : is_variadic_pack<Args...> {};

template <typename T>
inline constexpr auto is_group_v = typev<is_group<T>>;

template <typename... Args>
inline constexpr auto sizeof_v = sizeof...(Args);

template <typename T>
struct variadic_size;

template <template <typename...> typename T, typename... Args>
struct variadic_size<T<Args...>> {
  static constexpr auto value = sizeof_v<Args...>;
};

template <typename T>
inline constexpr auto variadic_size_v = typev<variadic_size<T>>;

template <typename T, typename = std::void_t<>>
struct sizeof_t : index_t<sizeof(T)> {};

template <auto N, typename T>
struct sizeof_t<index_type<N, T>> : sizeof_t<T> {};

template <typename T>
struct sizeof_t<T, std::void_t<decltype(T::size())>> : index_t<T::size()> {};

template <typename T>
struct sizeof_t<T, std::enable_if_t<is_variadic_type_v<T>>>
    : index_t<variadic_size_v<T>> {};

template <typename T>
inline constexpr auto sizeof_t_v = typev<sizeof_t<T>>;

template <typename T, typename U, auto p = sizeof_t_v<T>,
          auto q = sizeof_t_v<U>>
using less_t = bool_<(p < q)>;

template <typename T, typename U>
inline constexpr auto less_v = typev<less_t<T, U>>;

template <typename T, typename U, auto p = sizeof_t_v<T>,
          auto q = sizeof_t_v<U>>
using less_equal_t = bool_<(p <= q)>;

template <typename T, typename U>
inline constexpr auto less_equal_v = typev<less_equal_t<T, U>>;

template <typename T, typename U, auto p = sizeof_t_v<T>,
          auto q = sizeof_t_v<U>>
using equal_t = bool_<p == q>;

template <typename T, typename U>
inline constexpr auto equal_v = typev<equal_t<T, U>>;

template <typename T, typename U, auto p = sizeof_t_v<T>,
          auto q = sizeof_t_v<U>>
using greater_equal_t = bool_<(p >= q)>;

template <typename T, typename U>
inline constexpr auto greater_equal_v = typev<greater_equal_t<T, U>>;

template <typename T, typename U, auto p = sizeof_t_v<T>,
          auto q = sizeof_t_v<U>>
using greater_t = bool_<(p > q)>;

template <typename T, typename U>
inline constexpr auto greater_v = typev<greater_t<T, U>>;

template <typename T, typename U>
inline constexpr auto size_diff = sizeof_t_v<T> - sizeof_t_v<U>;

template <typename T>
struct clear : std::__type_identity<T> {};

template <template <typename...> typename T, typename... Args>
struct clear<T<Args...>> : std::__type_identity<T<>> {};

template <template <typename, auto...> typename T, typename U, auto... Args>
struct clear<T<U, Args...>> : std::__type_identity<T<U>> {};

template <typename T>
using clear_t = typeof_t<clear<T>>;

template <bool B, typename T>
struct clear_if : std::conditional_t<B, clear<T>, std::__type_identity<T>> {};

template <bool B, typename T>
using clear_if_t = typeof_t<clear_if<B, T>>;

template <auto value, decltype(value)... values>
struct homogeneous {
  using type = decltype(value);
  using value_type = type;

  static constexpr decltype(auto) size() { return sizeof...(values) + 1; }
};

template <size_t N, typename T>
struct get;

template <template <typename, auto...> typename T, typename U, auto value,
          auto... values>
struct get<0, T<U, value, values...>> : c_<value, U> {};

template <size_t N, template <typename, auto...> typename T, typename U,
          auto value, auto... values>
struct get<N, T<U, value, values...>>
    : c_<typev<get<N - 1, T<U, values...>>>, U> {};

template <template <auto...> typename T, auto value, auto... values>
struct get<0, T<value, values...>> : c_<value, int> {};

template <size_t N, template <auto...> typename T, auto value, auto... values>
struct get<N, T<value, values...>> : c_<typev<get<N - 1, T<values...>>>> {};

template <size_t N, typename T>
using get_t = typeof_t<get<N, T>>;

template <size_t N, typename T>
inline constexpr auto get_v = typev<get<N, T>>;

template <typename T, auto N = 0>
inline constexpr auto head_v = get_v<0, T> + N;

template <bool B, typename T>
struct collect;

template <bool B, template <typename, auto...> typename T, typename U,
          auto... values>
struct collect<B, T<U, values...>> : c_<B ? (values + ...) : (values*...), U> {
};

template <bool B, template <auto...> typename T, auto... values>
struct collect<B, T<values...>> : c_<B ? (values + ...) : (values*...)> {};

template <bool B, typename T>
using collect_t = typeof_t<collect<B, T>>;

template <bool B, typename T>
inline constexpr auto collect_v = typev<collect_t<B, T>>;

template <typename T>
struct sum : collect<1, T> {};

template <typename T>
using sum_t = typeof_t<sum<T>>;

template <typename T>
inline constexpr auto sum_v = typev<sum_t<T>>;

template <typename T>
struct mul : collect<0, T> {};

template <typename T>
using mul_t = typeof_t<mul<T>>;

template <typename T>
inline constexpr auto mul_v = typev<mul_t<T>>;

template <typename T>
struct means : c_<sum_v<T> / sizeof_t_v<T>> {};

template <typename T>
using means_t = typeof_t<means<T>>;

template <typename T>
inline constexpr auto means_v = typev<means_t<T>>;

template <typename L, typename R>
struct rename : std::__type_identity<L> {};

template <template <typename...> typename L, template <typename...> typename R,
          typename... T, typename... U>
struct rename<L<T...>, R<U...>> : std::__type_identity<R<T...>> {};

template <template <typename, auto...> typename L,
          template <typename, auto...> typename R, typename T, auto... t,
          typename U, auto... u>
struct rename<L<T, t...>, R<U, u...>> : std::__type_identity<R<U, t...>> {};

template <typename L, typename R>
using rename_t = typeof_t<rename<L, R>>;

template <bool B, typename L, typename R>
using rename_if = std::conditional_t<B, rename<L, R>, std::__type_identity<L>>;

template <bool B, typename L, typename R>
using rename_if_t = typeof_t<rename_if<B, L, R>>;

template <typename T, T... indices>
struct integer_sequence {
  using type = T;

  static constexpr decltype(auto) size() noexcept { return sizeof...(indices); }
};

template <typename T, typename U, bool B>
struct duple;

template <typename T, T... indices>
struct duple<T, integer_sequence<T, indices...>, false> {
  using type =
      integer_sequence<T, indices..., (sizeof...(indices) + indices)...>;
};

template <typename T, T... indices>
struct duple<T, integer_sequence<T, indices...>, true> {
  using type =
      integer_sequence<T, indices..., (sizeof...(indices) + indices)...,
                       sizeof...(indices) * 2>;
};

template <typename T, typename U, bool B>
using duple_t = typeof_t<duple<T, U, B>>;

template <typename T, size_t N>
struct next_integer_sequence
    : duple<T, typeof_t<next_integer_sequence<T, N / 2>>, N % 2> {};

template <typename T>
struct next_integer_sequence<T, 1> {
  using type = integer_sequence<T, 0>;
};

template <typename T>
struct next_integer_sequence<T, 0> {
  using type = integer_sequence<T>;
};

template <typename T, size_t N>
using next_integer_sequence_t = typeof_t<next_integer_sequence<T, N>>;

template <typename T, size_t N>
using make_integer_sequence = next_integer_sequence_t<T, N>;

template <typename T, typename... Args>
using integer_sequence_for = make_integer_sequence<T, sizeof_v<Args...>>;

template <size_t... indices>
using index_sequence = integer_sequence<size_t, indices...>;

template <size_t N>
using make_index_sequence = make_integer_sequence<size_t, N>;

template <typename... Args>
using index_sequence_for = make_index_sequence<sizeof_v<Args...>>;

template <auto N>
using index_sequence_of_c = std::make_index_sequence<N>;

template <typename T, auto N = 0>
using index_sequence_of_t = index_sequence_of_c<sizeof_t_v<T> - N>;

template <size_t, typename T = std::void_t<>>
using make_type = T;

template <typename T>
struct is_nest : std::false_type {};

template <template <typename...> typename T>
struct is_nest<T<>> : std::true_type {};

template <template <typename...> typename T, typename U>
struct is_nest<T<U>>
    : std::conditional_t<is_variadic_type_v<U>, is_nest<U>, std::true_type> {};

template <typename T>
inline constexpr auto is_nest_v = typev<is_nest<T>>;

template <auto N, typename... Args>
struct tuple_depth {
  static constexpr auto value = N * sizeof_v<Args...>;
};

template <auto N, template <typename...> typename T, typename... Args>
struct tuple_depth<N, T<Args...>> {
  static constexpr auto value = 1 + tuple_depth<N, Args...>::value;
};

template <auto N, typename T>
inline constexpr auto tuple_depth_v = typev<tuple_depth<N, T>>;

template <typename T>
struct nest_depth : tuple_depth<0, T> {};

template <typename T>
inline constexpr auto nest_depth_v = typev<nest_depth<T>>;

template <typename T>
struct nest_size : tuple_depth<1, T> {};

template <typename T>
inline constexpr auto nest_size_v = typev<nest_size<T>>;

template <auto N, typename T>
struct nest_element : std::__type_identity<T> {};

template <auto N, template <typename...> typename T, typename... Args>
struct nest_element<N, T<Args...>> {
  using next = alias_t<Args..., std::void_t<>>;
  using type = type_if<N != 0, nest_element<N - 1, next>,
                       std::__type_identity<T<Args...>>>;
};

template <auto N, typename T>
using nest_element_t = typeof_t<nest_element<N, T>>;

template <typename T>
struct nest_last : nest_element<nest_size_v<T> - 1, T> {};

template <typename T>
using nest_last_t = typeof_t<nest_last<T>>;

template <bool B, typename T>
using nest_last_if = std::conditional_t<B, nest_last<T>,
std::__type_identity<T>>;

template <bool B, typename T>
using nest_last_if_t = typeof_t<nest_last_if<B, T>>;

template <typename T, typename U>
using plus_t = c_<typev<T> + typev<U>>;

template <typename T, typename U>
inline constexpr auto plus_v = typev<plus_t<T, U>>;

template <typename T, typename U>
using minus_t = c_<typev<T> - typev<U>>;

template <typename T, typename U>
inline constexpr auto minus_v = typev<minus_t<T, U>>;

template <typename T, typename U>
using multiplies_t = c_<typev<T> * typev<U>>;

template <typename T, typename U>
inline constexpr auto multiplies_v = typev<multiplies_t<T, U>>;

template <typename T, typename U>
using divides_t = c_<typev<T> / typev<U>>;

template <typename T, typename U>
inline constexpr auto divides_v = typev<divides_t<T, U>>;

template <typename T, typename U>
using modulus_t = c_<typev<T> % typev<U>>;

template <typename T, typename U>
inline constexpr auto modulus_v = typev<modulus_t<T, U>>;

template <typename T>
using negate_t = c_<-typev<T>>;

template <typename T>
inline constexpr auto negate_v = typev<negate_t<T>>;

template <typename T>
struct pred : c_<typev<T> - 1, value_t<T>> {};

template <typename T>
using pred_t = typeof_t<pred<T>>;

template <typename T>
inline constexpr auto pred_v = typev<pred_t<T>>;

template <typename T>
struct succ : c_<typev<T> + 1, value_t<T>> {};

template <typename T>
using succ_t = typeof_t<succ<T>>;

template <typename T>
inline constexpr auto succ_v = typev<succ_t<T>>;

template <template <auto, typename> typename F, auto n, typename T, auto N = 1>
using prev = F<n - N, T>;

template <template <auto, typename> typename F, auto n, typename T, auto N = 1>
using next = F<n + N, T>;

template <auto n, typename T, auto N = 1>
inline constexpr auto prev_v = typev<prev<get, n, T, N>>;

template <auto n, typename T, auto N = 1>
inline constexpr auto next_v = typev<next<get, n, T, N>>;

template <auto x, auto y>
struct gcd : gcd<y, x % y> {};

template <auto y>
struct gcd<0, y> : c_<y> {};

template <auto x>
struct gcd<x, 0> : c_<x> {};

template <auto x, auto y>
using gcd_t = typeof_t<gcd<x, y>>;

template <auto x, auto y>
inline constexpr auto gcd_v = typev<gcd_t<x, y>>;

template <auto x, auto y>
struct lcm : c_<x * y / gcd_v<x, y>> {};

template <auto y>
struct lcm<0, y> : c_0 {};

template <auto x>
struct lcm<x, 0> : c_0 {};

template <auto x, auto y>
using lcm_t = typeof_t<lcm<x, y>>;

template <auto x, auto y>
inline constexpr auto lcm_v = typev<lcm_t<x, y>>;

template <template <typename...> typename F, typename T>
struct unpack;

template <template <typename...> typename F, template <typename...> typename T,
          typename... Args>
struct unpack<F, T<Args...>> {
  using type = F<Args...>;
};

template <template <typename...> typename F, typename T>
using unpack_t = typeof_t<unpack<F, T>>;

template <template <typename...> typename F, typename... Args>
struct bind_front {
  template <typename... args>
  using apply = F<Args..., args...>;
};

template <template <typename...> typename F, typename... Args>
struct bind_back {
  template <typename... args>
  using apply = F<args..., Args...>;
};

template <bool B1, bool B2, template <typename...> typename F, typename... Args>
struct bind_if {
  template <bool, typename... args>
  struct impl {
    using apply =
        std::conditional_t<B2, F<Args..., args...>, F<args..., Args...>>;
  };

  template <typename... args>
  struct impl<false, args...> {
    using apply = F<args...>;
  };

  template <typename... args>
  using apply = typename impl<B1, args...>::apply;
};

template <bool B, template <typename...> typename F, typename... Args>
struct bind_front_if : bind_if<B, 1, F, Args...> {};

template <bool B, template <typename...> typename F, typename... Args>
struct bind_back_if : bind_if<B, 0, F, Args...> {};

template <typename F, typename... Args>
using call = typename F::template apply<Args...>;

template <typename F, typename... Args>
using call_t = typeof_t<call<F, Args...>>;

template <bool B, typename T, typename F, typename... Args>
struct call_if {
  template <typename f, typename... args>
  struct impl {
    using type = call_t<f, args...>;
  };

  using type = type_if<B, impl<F, Args...>, std::__type_identity<T>>;
};

template <bool B, typename T, typename F, typename... Args>
using call_if_t = typeof_t<call_if<B, T, F, Args...>>;

template <typename F, typename... Args>
using invoke = call<F, Args...>;

template <typename F, typename... Args>
inline constexpr auto invoke_v = typev<invoke<F, Args...>>;

template <bool B, typename T, typename F, typename... Args>
struct invoke_if {
  template <typename f, typename... args>
  struct impl {
    using type = call<f, args...>;
  };

  using type = type_if<B, impl<F, Args...>, std::__type_identity<T>>;
};

template <bool B, typename T, typename F, typename... Args>
using invoke_if_t = typeof_t<invoke_if<B, T, F, Args...>>;

template <bool B, typename T, typename F, typename... Args>
inline constexpr auto invoke_if_v = typev<invoke_if_t<B, T, F, Args...>>;

template <typename F, typename T, typename...>
struct fold : std::__type_identity<T> {};

template <typename F, typename T, typename U, typename... Args>
struct fold<F, T, U, Args...> : fold<F, call_t<F, T, U>, Args...> {};

template <typename F, typename... Args>
using fold_t = typeof_t<fold<F, Args...>>;

template <template <typename...> typename F, typename... Args>
using folded = fold<bind_front<F>, Args...>;

template <template <typename...> typename F, typename... Args>
using folded_t = typeof_t<folded<F, Args...>>;

template <template <template <typename...> typename, typename...> typename F>
struct doll {
  template <typename... Args>
  struct apply {
    using type = typename F<apply, Args...>::type;
  };
};

template <template <template <typename...> typename, typename...> typename F,
          typename... Args>
using doll_t = typename doll<F>::template apply<Args...>::type;

template <template <typename...> typename F, template <typename...> typename T,
          typename... Args>
struct recurse : std::__type_identity<T<Args...>> {
  template <typename... args>
  using apply = recurse<F, T, Args..., typeof_t<F<args>>...>;
};

template <template <typename...> typename F, template <typename...> typename T,
          typename... Args>
using recurse_t = typeof_t<recurse<F, T, Args...>>;

template <typename F, typename...>
struct reduce : F {};

template <typename F, typename T, typename... Args>
struct reduce<F, T, Args...> : reduce<call<F, T>, Args...> {};

template <typename F, typename... Args>
using reduce_t = typeof_t<reduce<F, Args...>>;

template <template <typename...> typename F, template <typename...> typename T,
          typename... Args>
using curried = reduce<recurse<F, T>, Args...>;

template <template <typename...> typename F, template <typename...> typename T,
          typename... Args>
using curried_t = typeof_t<curried<F, T, Args...>>;

template <template <typename...> typename F, typename... Args>
using currying = curried<F, std::tuple, Args...>;

template <template <typename...> typename F, typename... Args>
using currying_t = typeof_t<currying<F, Args...>>;

template <typename T, template <typename...> typename... F>
struct compose_left;

template <typename T, template <typename...> typename F>
struct compose_left<T, F> : _typeof<F<T>> {};

template <typename T, template <typename...> typename F,
          template <typename...> typename... G>
struct compose_left<T, F, G...> : compose_left<typeof_t<F<T>>, G...> {};

template <typename T, template <typename...> typename... F>
using compose_left_t = typeof_t<compose_left<T, F...>>;

template <typename T, template <typename...> typename... F>
struct compose_right;

template <typename T, template <typename...> typename F>
struct compose_right<T, F> : _typeof<F<T>> {};

template <typename T, template <typename...> typename F,
          template <typename...> typename... G>
struct compose_right<T, F, G...>
    : compose_right<typeof_t<compose_right<T, G...>>, F> {};

template <typename T, template <typename...> typename... F>
using compose_right_t = typeof_t<compose_right<T, F...>>;

template <template <template <typename...> typename, typename...>
          typename transform,
          template <typename...> typename apply, typename T, typename... Args>
struct transform_apply {
  template <typename... args>
  using call = transform<apply, Args..., args...>;

  using type = unpack_t<call, T>;
};

template <template <template <typename...> typename, typename...>
          typename transform,
          template <typename...> typename apply, typename T, typename... Args>
using transform_apply_t =
    typeof_t<transform_apply<transform, apply, T, Args...>>;

template <typename T>
using tuple_and =
    transform_apply_t<folded_t, std::conjunction, T, std::true_type>;

template <typename T>
inline constexpr auto tuple_and_v = typev<tuple_and<T>>;

template <typename F, template <typename...> typename G>
struct on {
  template <typename... Args>
  using apply = G<call_t<F, Args>...>;
};

template <template <typename...> typename F, template <typename...> typename G,
          typename... Args>
using on_t = call_t<on<bind_front<F>, G>, Args...>;

template <typename F, typename... Args>
struct partial {
  template <typename... args>
  using apply = call<F, Args..., args...>;
};

template <template <typename...> typename F, bool neg = true>
struct negaf {
  template <typename... Args>
  using apply = wrapin_if<neg, std::negation, F<Args...>>;
};

template <template <template <typename...> typename, typename, int, int>
          typename F>
struct metaf {
  template <template <typename...> typename G, typename T, int B, int E>
  using apply = F<G, T, B, E>;
};

template <typename F, template <typename...> typename G, typename T, int B,
          int E>
using apply = typename F::template apply<G, T, B, E>;

template <typename F, template <typename...> typename G, typename T, int B,
          int E>
using apply_t = typeof_t<apply<F, G, T, B, E>>;

template <template <template <typename...> typename, typename, int, int>
          typename F,
          template <typename...> typename G, typename T, int B, int E,
          bool neg = false,
          template <typename...> typename U = negaf<G, neg>::template apply>
using applyf = apply<metaf<F>, U, T, B, E>;

template <template <template <typename...> typename, typename, int, int>
          typename F,
          template <typename...> typename G, typename T, int B, int E,
          bool neg = false>
using applyf_t = typeof_t<applyf<F, G, T, B, E, neg>>;

template <auto N>
struct equal {
  template <typename T>
  struct apply : bool_<typev<T> == N> {};
};

*/

}  // namespace meta

}  // namespace picidae

#endif  // PICIDAE_META_COMMON_HPP__
