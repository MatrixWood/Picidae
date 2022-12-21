#ifndef PICIDAE_DATE_DETAIL_DAY_HPP__
#define PICIDAE_DATE_DETAIL_DAY_HPP__

#include <sstream>

#include "picidae/date/detail/common.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

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

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_DAY_HPP__