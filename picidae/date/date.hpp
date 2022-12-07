#ifndef PICIDAE_DATE_DATE_HPP__
#define PICIDAE_DATE_DATE_HPP__

#include <algorithm>
#include <cassert>
#include <cctype>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <ios>
#include <istream>
#include <iterator>
#include <limits>
#include <locale>
#include <memory>
#include <ostream>
#include <ratio>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

PICIDAE_NAMESPACE_BEGIN(detail)

template <typename R1, typename R2>
using ratio_multiply = decltype(std::ratio_multiply<R1, R2>{});

template <typename R1, typename R2>
using ratio_divide = decltype(std::ratio_divide<R1, R2>{});

PICIDAE_NAMESPACE_END(detail)

using days = std::chrono::duration<
    int, detail::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

using weeks =
    std::chrono::duration<int,
                          detail::ratio_multiply<std::ratio<7>, days::period>>;

using years = std::chrono::duration<
    int, detail::ratio_multiply<std::ratio<146097, 400>, days::period>>;

using months =
    std::chrono::duration<int,
                          detail::ratio_divide<years::period, std::ratio<12>>>;

class day;

day operator+(const day& x, const days& y) noexcept;
day operator+(const days& x, const day& y) noexcept;
day operator-(const day& x, const days& y) noexcept;
days operator-(const day& x, const day& y) noexcept;

class day {
 public:
  day() = default;
  inline explicit constexpr day(unsigned d) noexcept : _d(d) {}

  inline day& operator++() noexcept {
    ++_d;
    return *this;
  }

  inline day operator++(int) noexcept {
    auto tmp(*this);
    ++(*this);
    return tmp;
  }

  inline day& operator--() noexcept {
    --_d;
    return *this;
  }

  inline day operator--(int) noexcept {
    auto tmp(*this);
    --(*this);
    return tmp;
  }

  inline day& operator+=(const days& d) noexcept {
    *this = *this + d;
    return *this;
  }

  inline day& operator-=(const days& d) noexcept {
    *this = *this - d;
    return *this;
  }


  inline constexpr operator unsigned() const noexcept {
    return _d;
  }

  constexpr bool ok() const noexcept {
    return 1 <= _d && _d <= 31;
  }

 private:
  unsigned char _d;
};

inline day operator+(const day& x, const days& y) noexcept {
  return day{static_cast<unsigned>(x) + static_cast<unsigned>(y.count())};
}

inline day operator+(const days& x, const day& y) noexcept {
  return y + x;
}

inline days operator-(const day& x, const day& y) noexcept {
  return days{static_cast<unsigned>(x) + static_cast<unsigned>(y)};
}

inline day operator-(const day& x, const days& y) noexcept {
  return x + -y;
}

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DATE_HPP__