#ifndef PICIDAE_META_DESCR_HPP__
#define PICIDAE_META_DESCR_HPP__

#include <sys/types.h>

#include <cstddef>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "picidae/meta/common.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(meta)

template <size_t N, typename... Ts>
struct descr {
  char text[N + 1]{'\0'};

  constexpr descr() = default;

  constexpr descr(const char (&s)[N + 1])
      : descr(s, std::make_index_sequence<N>()) {}

  template <size_t... Is>
  constexpr descr(const char (&s)[N + 1], meta::index_sequence<Is...>)
      : text{s[Is]..., '\0'} {}

  template <typename... Chars>
  constexpr descr(char c, Chars... cs)
      : text{c, static_cast<char>(cs)..., '\0'} {}

  constexpr static std::array<const std::type_info*, sizeof...(Ts) + 1>
  types() {
    return {{&typeid(Ts)..., nullptr}};
  }
};

template <size_t N1, size_t N2, typename... Ts1, typename... Ts2, size_t... Is1,
          size_t... Is2>
constexpr descr<N1 + N2, Ts1..., Ts2...> plus_impl(
    const descr<N1, Ts1...>& a, const descr<N2, Ts2...>& b,
    meta::index_sequence<Is1...>, meta::index_sequence<Is2...>) {
  return {a.text[Is1]..., b.text[Is2]...};
}

template <size_t N1, size_t N2, typename... Ts1, typename... Ts2>
constexpr descr<N1 + N2, Ts1..., Ts2...> operator+(const descr<N1, Ts1...>& a,
                                                   const descr<N2, Ts2...>& b) {
  return plus_impl(a, b, meta::make_index_sequence<N1>(),
                   meta::make_index_sequence<N2>());
}

template <size_t N>
constexpr descr<N - 1> const_name(const char (&text)[N]) {
  return descr<N - 1>(text);
}

constexpr descr<0> const_name(const char (&)[1]) { return {}; }

template <bool B, size_t N1, size_t N2>
constexpr std::enable_if_t<B, descr<N1 - 1>> const_name(const char (&text1)[N1],
                                                        const char (&)[N2]) {
  return const_name(text1);
}

template <bool B, size_t N1, size_t N2>
constexpr std::enable_if_t<!B, descr<N2 - 1>> const_name(
    const char (&)[N1], const char (&text2)[N2]) {
  return const_name(text2);
}

template <bool B, typename T1, typename T2>
constexpr std::enable_if_t<B, T1> const_name(const T1& d, const T2&) {
  return d;
}

template <bool B, typename T1, typename T2>
constexpr std::enable_if_t<!B, T2> const_name(const T1&, const T2& d) {
  return d;
}

template <size_t Rem, size_t... Digits>
struct int_to_str : int_to_str<Rem / 10, Rem % 10, Digits...> {};
template <size_t... Digits>
struct int_to_str<0, Digits...> {
  static constexpr auto digits = descr<sizeof...(Digits)>(('0' + Digits)...);
};

template <size_t Size>
auto constexpr const_name()
    -> std::remove_cv_t<decltype(int_to_str<Size / 10, Size % 10>::digits)> {
  return int_to_str<Size / 10, Size % 10>::digits;
}

template <typename Type>
constexpr descr<1, Type> const_name() {
  return {'%'};
}

template <size_t N>
constexpr descr<N - 1> _(char const (&text)[N]) {
  return const_name<N>(text);
}
template <bool B, size_t N1, size_t N2>
constexpr std::enable_if_t<B, descr<N1 - 1>> _(char const (&text1)[N1],
                                               char const (&text2)[N2]) {
  return const_name<B, N1, N2>(text1, text2);
}
template <bool B, size_t N1, size_t N2>
constexpr std::enable_if_t<!B, descr<N2 - 1>> _(char const (&text1)[N1],
                                                char const (&text2)[N2]) {
  return const_name<B, N1, N2>(text1, text2);
}
template <bool B, typename T1, typename T2>
constexpr std::enable_if_t<B, T1> _(const T1& d1, const T2& d2) {
  return const_name<B, T1, T2>(d1, d2);
}
template <bool B, typename T1, typename T2>
constexpr std::enable_if_t<!B, T2> _(const T1& d1, const T2& d2) {
  return const_name<B, T1, T2>(d1, d2);
}

template <size_t Size>
auto constexpr _()
    -> std::remove_cv_t<decltype(int_to_str<Size / 10, Size % 10>::digits)> {
  return const_name<Size>();
}
template <typename Type>
constexpr descr<1, Type> _() {
  return const_name<Type>();
}

constexpr descr<0> concat() { return {}; }

template <size_t N, typename... Ts>
constexpr descr<N, Ts...> concat(const descr<N, Ts...>& descr) {
  return descr;
}

template <size_t N, typename... Ts, typename... Args>
constexpr auto concat(const descr<N, Ts...>& d, const Args&... args)
    -> decltype(std::declval<descr<N + 2, Ts...>>() + concat(args...)) {
  return d + const_name(", ") + concat(args...);
}

template <size_t N, typename... Ts>
constexpr descr<N + 2, Ts...> type_descr(const descr<N, Ts...>& descr) {
  return const_name("{") + descr + const_name("}");
}

PICIDAE_NAMESPACE_END(meta)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_DESCR_HPP__