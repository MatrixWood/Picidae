#ifndef PICIDAE_DATE_DETAIL_YEAR_HPP__
#define PICIDAE_DATE_DETAIL_YEAR_HPP__

#include <sstream>

#include "picidae/date/detail/common.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

class year;

constexpr year operator+(const year& x, const years& y) noexcept;
constexpr year operator+(const years& x, const year& y) noexcept;
constexpr years operator+(const year& x, const year& y) noexcept;
constexpr year operator-(const year& x, const years& y) noexcept;
constexpr years operator-(const year& x, const year& y) noexcept;

class year {
 public:
  year() = default;

  inline explicit constexpr year(int y) noexcept
      : _y(static_cast<decltype(_y)>(y)) {}

  inline year& operator++() noexcept {
    ++_y;
    return *this;
  }

  inline year operator++(int) noexcept {
    auto tmp(*this);
    ++(*this);
    return tmp;
  }

  inline year& operator--() noexcept {
    --_y;
    return *this;
  }

  inline year operator--(int) noexcept {
    auto tmp(*this);
    --(*this);
    return tmp;
  }

  inline year& operator+=(const years& m) noexcept {
    *this = *this + m;
    return *this;
  }

  inline year& operator-=(const years& m) noexcept {
    *this = *this - m;
    return *this;
  }

  inline constexpr year operator-() const noexcept { return year{-_y}; }

  inline constexpr year operator+() const noexcept { return *this; }

  inline constexpr bool is_leap() const noexcept {
    return _y % 4 == 0 && (_y % 100 != 0 || _y % 400 == 0);
  }

  inline constexpr operator int() const noexcept { return _y; }

  constexpr bool ok() const noexcept {
    return _y != std::numeric_limits<short>::min();
  }

  static constexpr year min() noexcept { return year{-32767}; }
  static constexpr year max() noexcept { return year{32767}; }

 private:
  short _y;
};

inline constexpr year operator+(const year& x, const years& y) noexcept {
  return year{static_cast<int>(x) + y.count()};
}

inline constexpr year operator+(const years& x, const year& y) noexcept { return y + x; }

inline constexpr years operator-(const year& x, const year& y) noexcept {
  return years{static_cast<int>(x) - static_cast<int>(y)};
}

inline constexpr year operator-(const year& x, const years& y) noexcept { return x + -y; }

inline constexpr year operator"" _y(unsigned long long y) noexcept {
  return year(static_cast<int>(y));
}

inline constexpr bool operator==(const year& x, const year& y) noexcept {
  return static_cast<int>(x) == static_cast<int>(y);
}

inline constexpr bool operator!=(const year& x, const year& y) noexcept {
  return !(x == y);
}

inline constexpr bool operator<(const year& x, const year& y) noexcept {
  return static_cast<int>(x) < static_cast<int>(y);
}

inline constexpr bool operator>(const year& x, const year& y) noexcept {
  return y < x;
}

inline constexpr bool operator<=(const year& x, const year& y) noexcept {
  return !(y < x);
}

inline constexpr bool operator>=(const year& x, const year& y) noexcept {
  return !(x < y);
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const year& y) {
  os.fill('0');
  os.flags(std::ios::dec | std::ios::internal);
  os.width(4 + (y < year{0}));
  os.imbue(std::locale::classic());
  os << static_cast<int>(y);
  return os;
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const year& y) {
  detail::low_level_fmt(os, y);
  if (!y.ok()) os << " is not a valid year";
  return os;
}

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_YEAR_HPP__