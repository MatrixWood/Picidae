#ifndef PICIDAE_DATE_DETAIL_MONTH_HPP__
#define PICIDAE_DATE_DETAIL_MONTH_HPP__

#include <sstream>

#include "picidae/date/detail/common.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

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
    // os << format(os.getloc(), fmt, m);
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

constexpr const date::month jan{1};
constexpr const date::month feb{2};
constexpr const date::month mar{3};
constexpr const date::month apr{4};
constexpr const date::month may{5};
constexpr const date::month jun{6};
constexpr const date::month jul{7};
constexpr const date::month aug{8};
constexpr const date::month sep{9};
constexpr const date::month oct{10};
constexpr const date::month nov{11};
constexpr const date::month dec{12};

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_MONTH_HPP__