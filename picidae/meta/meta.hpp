#ifndef PICIDAE_META_HPP__
#define PICIDAE_META_HPP__

#include <tuple>
#include <utility>
#include <variant>
#include <functional>

#include "common.hpp"

namespace picidae {
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

} // namespace picidae

#endif // PICIDAE_META_HPP__
