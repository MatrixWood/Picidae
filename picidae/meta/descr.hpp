#ifndef PICIDAE_META_DESCR_HPP__
#define PICIDAE_META_DESCR_HPP__

#include "picidae/meta/macro.hpp"
#include "picidae/meta/common.hpp"
#include <sys/types.h>
#include <typeinfo>
#include <utility>

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(detail)

template <size_t N, typename... Ts>
struct descr {
  char text[N + 1]{'\0'};

  constexpr descr() = default;

  constexpr descr(const char (&s)[N + 1]) : descr(s, std::make_index_sequence<N>()) {}

  template <size_t... Is>
  constexpr descr(const char (&s)[N + 1], std::index_sequence<Is...>) : text{s[Is]..., '\0'} {}

  template <typename... Chars>
  constexpr descr(char c, Chars... cs) : text{c, static_cast<char>(cs)..., '\0'} {}

  constexpr static std::array<const std::type_info*, sizeof...(Ts) + 1> types() {
    return {{&typeid(Ts)..., nullptr}};
  }
};

PICIDAE_NAMESPACE_END(detail)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_META_DESCR_HPP__ 