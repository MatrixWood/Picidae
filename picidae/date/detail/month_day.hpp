#ifndef PICIDAE_DATE_DETAIL_MONTH_DAY_HPP__
#define PICIDAE_DATE_DETAIL_MONTH_DAY_HPP__

#include <sstream>

#include "picidae/date/detail/common.hpp"
#include "picidae/date/detail/day.hpp"
#include "picidae/date/detail/month.hpp"
#include "picidae/date/detail/operator.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

class month_day {
 public:
  month_day() = default;
  constexpr month_day(const date::month& m, const date::day& d) noexcept
      : _m(m), _d(d) {}

  constexpr date::month month() const noexcept { return _m; }

  constexpr date::day day() const noexcept { return _d; }

  constexpr bool ok() const noexcept {
    constexpr const date::day d[] = {
        date::day(31), date::day(29), date::day(31), date::day(30),
        date::day(31), date::day(30), date::day(31), date::day(31),
        date::day(30), date::day(31), date::day(30), date::day(31)};
    return _m.ok() && date::day{1} <= _d &&
           _d <= d[static_cast<unsigned>(_m) - 1];
  }

 private:
  date::month _m;
  date::day _d;
};

constexpr month_day operator/(const day& d, const month& m) noexcept {
  return m / d;
}

constexpr month_day operator/(const day& d, int m) noexcept {
  return m / d;
}

constexpr month_day operator/(const month& m, const day& d) noexcept {
  return {m, d};
}

constexpr month_day operator/(const month& m, int d) noexcept {
  return m / day(static_cast<unsigned>(d));
}

constexpr month_day operator/(int m, const day& d) noexcept {
  return month(static_cast<unsigned>(m)) / d;
}

constexpr bool operator==(const month_day& x, const month_day& y) noexcept {
  return x.month() == y.month() && x.day() == y.day();
}

constexpr bool operator!=(const month_day& x, const month_day& y) noexcept {
  return !(x == y);
}

constexpr bool operator<(const month_day& x, const month_day& y) noexcept {
  return x.month() < y.month()
             ? true
             : (x.month() > y.month() ? false : (x.day() < y.day()));
}

constexpr bool operator>(const month_day& x, const month_day& y) noexcept {
  return y < x;
}

constexpr bool operator<=(const month_day& x, const month_day& y) noexcept {
  return !(y < x);
}

constexpr bool operator>=(const month_day& x, const month_day& y) noexcept {
  return !(x < y);
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const month_day& md) {
  low_level_fmt(os, md.month()) << '/';
  return low_level_fmt(os, md.day());
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const month_day& md) {
  detail::low_level_fmt(os, md);
  if (!md.ok()) os << " is not a valid month_day";
  return os;
}

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_MONTH_DAY_HPP__