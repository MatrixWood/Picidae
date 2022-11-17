#ifndef PICIDAE_JSON_DETAIL_PARSE_NUMBER_HPP__
#define PICIDAE_JSON_DETAIL_PARSE_NUMBER_HPP__

#include <cmath>
#include <string>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(json)

PICIDAE_NAMESPACE_BEGIN(detail)

template <typename T>
[[nodiscard]] constexpr auto parse_num(const std::string_view t_str) noexcept ->
    typename std::enable_if<std::is_integral<T>::value, T>::type {
  T t = 0;
  for (const auto c : t_str) {
    if (c < '0' || c > '9') {
      return t;
    }
    t *= 10;
    t += c - '0';
  }
  return t;
}

template <typename T>
[[nodiscard]] auto parse_num(const std::string_view t_str) ->
    typename std::enable_if<!std::is_integral<T>::value, T>::type {
  T t = 0;
  T base{};
  T decimal_place = 0;
  int exponent = 0;

  for (const auto c : t_str) {
    switch (c) {
      case '.':
        decimal_place = 10;
        break;
      case 'e':
      case 'E':
        exponent = 1;
        decimal_place = 0;
        base = t;
        t = 0;
        break;
      case '-':
        exponent = -1;
        break;
      case '+':
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if (decimal_place < 10) {
          t *= 10;
          t += static_cast<T>(c - '0');
        } else {
          t += static_cast<T>(c - '0') / decimal_place;
          decimal_place *= 10;
        }
        break;
      default:
        break;
    }
  }
  return exponent ? base * std::pow(T(10), t * static_cast<T>(exponent)) : t;
}

PICIDAE_NAMESPACE_END(detal)

PICIDAE_NAMESPACE_END(json)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_JSON_DETAIL_PARSE_NUMBER_HPP__
