#ifndef PICIDAE_DATE_DETAIL_YEAR_MONTH_HPP__
#define PICIDAE_DATE_DETAIL_YEAR_MONTH_HPP__

#include <sstream>

#include "picidae/date/detail/common.hpp"
#include "picidae/date/detail/month.hpp"
#include "picidae/date/detail/operator.hpp"
#include "picidae/date/detail/year.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

class year_month;

template <class = detail::unspecified_month_disambiguator>
year_month operator+(const year_month& ym, const months& dm) noexcept;
template <class = detail::unspecified_month_disambiguator>
year_month operator+(const months& dm, const year_month& ym) noexcept;
template <class = detail::unspecified_month_disambiguator>
year_month operator-(const year_month& ym, const months& dm) noexcept;

class year_month {
 public:
  year_month() = default;
  constexpr year_month(const date::year& y, const date::month& m) noexcept
      : _y(y), _m(m) {}

  constexpr date::year year() const noexcept { return _y; }

  constexpr date::month month() const noexcept { return _m; }

  template <class = detail::unspecified_month_disambiguator>
  year_month& operator+=(const months& dm) noexcept {
    *this = *this + dm;
    return *this;
  }

  template <class = detail::unspecified_month_disambiguator>
  year_month& operator-=(const months& dm) noexcept {
    *this = *this - dm;
    return *this;
  }

  year_month& operator+=(const years& dy) noexcept {
    *this = *this + dy;
    return *this;
  }
  year_month& operator-=(const years& dy) noexcept {
    *this = *this - dy;
    return *this;
  }

  constexpr bool ok() const noexcept { return _y.ok() && _m.ok(); }

 private:
  date::year _y;
  date::month _m;
};

constexpr bool operator==(const year_month& x, const year_month& y) noexcept {
  return x.year() == y.year() && x.month() == y.month();
}

constexpr bool operator!=(const year_month& x, const year_month& y) noexcept {
  return !(x == y);
}

constexpr bool operator<(const year_month& x, const year_month& y) noexcept {
  return x.year() < y.year()
             ? true
             : (x.year() > y.year() ? false : (x.month() < y.month()));
}

constexpr bool operator>(const year_month& x, const year_month& y) noexcept {
  return y < x;
}

constexpr bool operator<=(const year_month& x, const year_month& y) noexcept {
  return !(y < x);
}

constexpr bool operator>=(const year_month& x, const year_month& y) noexcept {
  return !(x < y);
}

constexpr year_month operator/(const year& y, const month& m) noexcept {
  return {y, m};
}

constexpr year_month operator/(const year& y, int m) noexcept {
  return y / month(static_cast<unsigned>(m));
}

template <class>
year_month operator+(const year_month& ym, const months& dm) noexcept {
  auto dmi =
      static_cast<int>(static_cast<unsigned>(ym.month())) - 1 + dm.count();
  auto dy = (dmi >= 0 ? dmi : dmi - 11) / 12;
  dmi = dmi - dy * 12 + 1;
  return (ym.year() + years(dy)) / month(static_cast<unsigned>(dmi));
}

template <class>
year_month operator+(const months& dm, const year_month& ym) noexcept {
  return ym + dm;
}

template <class>
year_month operator-(const year_month& ym, const months& dm) noexcept {
  return ym + -dm;
}

constexpr months operator-(const year_month& x, const year_month& y) noexcept {
  return (x.year() - y.year()) + months(static_cast<unsigned>(x.month()) -
                                        static_cast<unsigned>(y.month()));
}

constexpr year_month operator+(const year_month& ym, const years& dy) noexcept {
  return (ym.year() + dy) / ym.month();
}

constexpr year_month operator+(const years& dy, const year_month& ym) noexcept {
  return ym + dy;
}

constexpr year_month operator-(const year_month& ym, const years& dy) noexcept {
  return ym + -dy;
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const year_month& ym) {
  low_level_fmt(os, ym.year()) << '/';
  return low_level_fmt(os, ym.month());
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const year_month& ym) {
  detail::low_level_fmt(os, ym);
  if (!ym.ok()) os << " is not a valid year_month";
  return os;
}

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_YEAR_MONTH_HPP__