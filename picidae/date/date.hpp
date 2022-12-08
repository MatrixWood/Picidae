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

#include "picidae/date/stream.hpp"
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

// day

class day;

day operator+(const day& x, const days& y) noexcept;
day operator+(const days& x, const day& y) noexcept;
days operator+(const day& x, const day& y) noexcept;
day operator-(const day& x, const days& y) noexcept;
days operator-(const day& x, const day& y) noexcept;

class day {
 public:
  day() = default;
  inline explicit constexpr day(unsigned d) noexcept
      : _d(static_cast<decltype(_d)>(d)) {}

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

  inline constexpr operator unsigned() const noexcept { return _d; }

  constexpr bool ok() const noexcept { return 1 <= _d && _d <= 31; }

 private:
  unsigned char _d;
};

inline day operator+(const day& x, const days& y) noexcept {
  return day{static_cast<unsigned>(x) + static_cast<unsigned>(y.count())};
}

inline day operator+(const days& x, const day& y) noexcept { return y + x; }

inline days operator+(const day& x, const day& y) noexcept {
  return days{static_cast<unsigned>(x) + static_cast<unsigned>(y)};
}

inline days operator-(const day& x, const day& y) noexcept {
  return days{static_cast<unsigned>(x) - static_cast<unsigned>(y)};
}

inline day operator-(const day& x, const days& y) noexcept { return x + -y; }

inline constexpr day operator"" _d(unsigned long long d) noexcept {
  return day{static_cast<unsigned>(d)};
}

inline constexpr bool operator==(const day& x, const day& y) noexcept {
  return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

inline constexpr bool operator!=(const day& x, const day& y) noexcept {
  return !(x == y);
}

inline constexpr bool operator<(const day& x, const day& y) noexcept {
  return static_cast<unsigned>(x) < static_cast<unsigned>(y);
}

inline constexpr bool operator>(const day& x, const day& y) noexcept {
  return y < x;
}

inline constexpr bool operator<=(const day& x, const day& y) noexcept {
  return !(y < x);
}

inline constexpr bool operator>=(const day& x, const day& y) noexcept {
  return !(x < y);
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const day& d) {
  // stream::save_ostream<CharT, Traits> _(os);
  os.fill('0');
  os.flags(std::ios::dec | std::ios::right);
  os.width(2);
  os << static_cast<unsigned>(d);
  return os;
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const day& d) {
  detail::low_level_fmt(os, d);
  if (!d.ok()) os << " is not a valid day";
  return os;
}

// month

class month;

month operator+(const month& x, const months& y) noexcept;
month operator+(const months& x, const month& y) noexcept;
months operator+(const month& x, const month& y) noexcept;
month operator-(const month& x, const months& y) noexcept;
months operator-(const month& x, const month& y) noexcept;

class month {
 public:
  month() = default;

  inline explicit constexpr month(unsigned m) noexcept
      : _m(static_cast<decltype(_m)>(m)) {}

  inline month& operator++() noexcept {
    ++_m;
    return *this;
  }

  inline month operator++(int) noexcept {
    auto tmp(*this);
    ++(*this);
    return tmp;
  }

  inline month& operator--() noexcept {
    --_m;
    return *this;
  }

  inline month operator--(int) noexcept {
    auto tmp(*this);
    --(*this);
    return tmp;
  }

  inline month& operator+=(const months& m) noexcept {
    *this = *this + m;
    return *this;
  }

  inline month& operator-=(const months& m) noexcept {
    *this = *this - m;
    return *this;
  }

  inline constexpr operator unsigned() const noexcept { return _m; }

  constexpr bool ok() const noexcept { return 1 <= _m && _m <= 12; }

 private:
  unsigned char _m;
};

inline month operator+(const month& x, const months& y) noexcept {
  const auto mu =
      static_cast<long long>(static_cast<unsigned>(x)) + y.count() - 1;
  const auto yr = (mu >= 0 ? mu : mu - 11) / 12;
  return month{static_cast<unsigned>(mu - yr * 12 + 1)};
}

inline month operator+(const months& x, const month& y) noexcept {
  return y + x;
}

inline months operator+(const month& x, const month& y) noexcept {
  const auto d = static_cast<unsigned>(x) + static_cast<unsigned>(y);
  auto const mu = static_cast<long long>(d) - 1;
  auto const yr = (mu >= 0 ? mu : mu - 11) / 12;
  return months(
      static_cast<unsigned>(1 <= d && d <= 12 ? d : mu - yr * 12 + 1));
}

inline months operator-(const month& x, const month& y) noexcept {
  const auto d = static_cast<unsigned>(x) - static_cast<unsigned>(y);
  return months(static_cast<unsigned>(d == 0 ? d + 12 : d));
}

inline month operator-(const month& x, const months& y) noexcept {
  return x + -y;
}

inline month operator"" _m(unsigned long long m) noexcept {
  auto const mu = static_cast<long long>(static_cast<unsigned>(m)) - 1;
  auto const yr = (mu >= 0 ? mu : mu - 11) / 12;
  return month{static_cast<unsigned>(mu - yr * 12 + 1)};
}

inline constexpr bool operator==(const month& x, const month& y) noexcept {
  return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

inline constexpr bool operator!=(const month& x, const month& y) noexcept {
  return !(x == y);
}

inline constexpr bool operator<(const month& x, const month& y) noexcept {
  return static_cast<unsigned>(x) < static_cast<unsigned>(y);
}

inline constexpr bool operator>(const month& x, const month& y) noexcept {
  return y < x;
}

inline constexpr bool operator<=(const month& x, const month& y) noexcept {
  return !(y < x);
}

inline constexpr bool operator>=(const month& x, const month& y) noexcept {
  return !(x < y);
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const month& m) {
  if (m.ok()) {
    CharT fmt[] = {'%', 'b', 0};
    //os << format(os.getloc(), fmt, m);
    os << static_cast<unsigned>(m);
  } else
    os << static_cast<unsigned>(m);
  return os;
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const month& m) {
  detail::low_level_fmt(os, m);
  if (!m.ok()) os << " is not a valid month";
  return os;
}

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DATE_HPP__