#ifndef PICIDAE_DATE_DETAIL_YEAR_MONTH_DAY_HPP__
#define PICIDAE_DATE_DETAIL_YEAR_MONTH_DAY_HPP__

#include <sstream>

#include "picidae/date/detail/common.hpp"
#include "picidae/date/detail/day.hpp"
#include "picidae/date/detail/month.hpp"
#include "picidae/date/detail/month_day.hpp"
#include "picidae/date/detail/operator.hpp"
#include "picidae/date/detail/year.hpp"
#include "picidae/date/detail/year_month.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

template <class = detail::unspecified_month_disambiguator>
year_month_day operator+(const year_month_day& ymd, const months& dm) noexcept;
template <class = detail::unspecified_month_disambiguator>
year_month_day operator+(const months& dm, const year_month_day& ymd) noexcept;
template <class = detail::unspecified_month_disambiguator>
year_month_day operator-(const year_month_day& ymd, const months& dm) noexcept;
constexpr year_month_day operator+(const year_month_day& ymd,
                                   const years& dy) noexcept;
constexpr year_month_day operator+(const years& dy,
                                   const year_month_day& ymd) noexcept;
constexpr year_month_day operator-(const year_month_day& ymd,
                                   const years& dy) noexcept;

class year_month_day {
 public:
  year_month_day() = default;
  constexpr year_month_day(const date::year& y, const date::month& m,
                           const date::day& d) noexcept
      : _y(y), _m(m), _d(d) {}

  year_month_day(const year_month_day_last& ymdl) noexcept;

  year_month_day(sys_days dp) noexcept
      : year_month_day(from_days(dp.time_since_epoch())) {}
  explicit year_month_day(local_days dp) noexcept
      : year_month_day(from_days(dp.time_since_epoch())) {}

  template <class = detail::unspecified_month_disambiguator>
  year_month_day& operator+=(const months& m) noexcept {
    *this = *this + m;
    return *this;
  }

  template <class = detail::unspecified_month_disambiguator>
  year_month_day& operator-=(const months& m) noexcept {
    *this = *this - m;
    return *this;
  }

  year_month_day& operator+=(const years& y) noexcept {
    *this = *this + y;
    return *this;
  }
  year_month_day& operator-=(const years& y) noexcept {
    *this = *this - y;
    return *this;
  }

  constexpr date::year year() const noexcept { return _y; }
  constexpr date::month month() const noexcept { return _m; }
  constexpr date::day day() const noexcept { return _d; }

  operator sys_days() const noexcept { return sys_days{to_days()}; }

  explicit operator local_days() const noexcept {
    return local_days{to_days()};
  }

  constexpr bool ok() const noexcept;

 private:
  static year_month_day from_days(days dp) noexcept;
  days to_days() const noexcept;

 private:
  date::year _y;
  date::month _m;
  date::day _d;
};

inline days year_month_day::to_days() const noexcept {
  static_assert(
      std::numeric_limits<unsigned>::digits >= 18,
      "This algorithm has not been ported to a 16 bit unsigned integer");
  static_assert(
      std::numeric_limits<int>::digits >= 20,
      "This algorithm has not been ported to a 16 bit signed integer");
  auto const y = static_cast<int>(_y) - (_m <= February);
  auto const m = static_cast<unsigned>(_m);
  auto const d = static_cast<unsigned>(_d);
  auto const era = (y >= 0 ? y : y - 399) / 400;
  auto const yoe = static_cast<unsigned>(y - era * 400);             // [0, 399]
  auto const doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;  // [0, 365]
  auto const doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;  // [0, 146096]
  return days{era * 146097 + static_cast<int>(doe) - 719468};
}

inline year_month_day year_month_day::from_days(days dp) noexcept {
  static_assert(
      std::numeric_limits<unsigned>::digits >= 18,
      "This algorithm has not been ported to a 16 bit unsigned integer");
  static_assert(
      std::numeric_limits<int>::digits >= 20,
      "This algorithm has not been ported to a 16 bit signed integer");
  auto const z = dp.count() + 719468;
  auto const era = (z >= 0 ? z : z - 146096) / 146097;
  auto const doe = static_cast<unsigned>(z - era * 146097);  // [0, 146096]
  auto const yoe =
      (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
  auto const y = static_cast<days::rep>(yoe) + era * 400;
  auto const doy = doe - (365 * yoe + yoe / 4 - yoe / 100);  // [0, 365]
  auto const mp = (5 * doy + 2) / 153;                       // [0, 11]
  auto const d = doy - (153 * mp + 2) / 5 + 1;               // [1, 31]
  auto const m = mp < 10 ? mp + 3 : mp - 9;                  // [1, 12]
  return year_month_day{date::year{y + (m <= 2)}, date::month(m), date::day(d)};
}

constexpr year_month_day operator/(const year_month& ym,
                                   const day& d) noexcept {
  return {ym.year(), ym.month(), d};
}

constexpr year_month_day operator/(const year_month& ym, int d) noexcept {
  return ym / day(static_cast<unsigned>(d));
}

constexpr year_month_day operator/(const year& y,
                                   const month_day& md) noexcept {
  return y / md.month() / md.day();
}

constexpr year_month_day operator/(int y, const month_day& md) noexcept {
  return year(y) / md;
}

constexpr year_month_day operator/(const month_day& md,
                                   const year& y) noexcept {
  return y / md;
};

constexpr year_month_day operator/(const month_day& md, int y) noexcept {
  return y / md;
}

constexpr bool operator==(const year_month_day& x,
                          const year_month_day& y) noexcept {
  return x.year() == y.year() && x.month() == y.month() && x.day() == y.day();
}

constexpr bool operator!=(const year_month_day& x,
                          const year_month_day& y) noexcept {
  return !(x == y);
}

constexpr bool operator<(const year_month_day& x,
                         const year_month_day& y) noexcept {
  return x.year() < y.year()
             ? true
             : (x.year() > y.year()
                    ? false
                    : (x.month() < y.month()
                           ? true
                           : (x.month() > y.month() ? false
                                                    : (x.day() < y.day()))));
}

constexpr bool operator>(const year_month_day& x,
                         const year_month_day& y) noexcept {
  return y < x;
}

constexpr bool operator<=(const year_month_day& x,
                          const year_month_day& y) noexcept {
  return !(y < x);
}

constexpr bool operator>=(const year_month_day& x,
                          const year_month_day& y) noexcept {
  return !(x < y);
}

template <class>
year_month_day operator+(const year_month_day& ymd, const months& dm) noexcept {
  return (ymd.year() / ymd.month() + dm) / ymd.day();
}

template <class>
year_month_day operator+(const months& dm, const year_month_day& ymd) noexcept {
  return ymd + dm;
}

template <class>
year_month_day operator-(const year_month_day& ymd, const months& dm) noexcept {
  return ymd + (-dm);
}

constexpr year_month_day operator+(const year_month_day& ymd,
                                   const years& dy) noexcept {
  return (ymd.year() + dy) / ymd.month() / ymd.day();
}

constexpr year_month_day operator+(const years& dy,
                                   const year_month_day& ymd) noexcept {
  return ymd + dy;
}

constexpr year_month_day operator-(const year_month_day& ymd,
                                   const years& dy) noexcept {
  return ymd + -dy;
}

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const year_month_day& ymd) {
  os.fill('0');
  os.flags(std::ios::dec | std::ios::right);
  os.imbue(std::locale::classic());
  os << static_cast<int>(ymd.year()) << '-';
  os.width(2);
  os << static_cast<unsigned>(ymd.month()) << '-';
  os.width(2);
  os << static_cast<unsigned>(ymd.day());
  if (!ymd.ok()) os << " is not a valid year_month_day";
  return os;
}

template <class = detail::unspecified_month_disambiguator>
year_month_day_last operator+(const year_month_day_last& ymdl,
                              const months& dm) noexcept;

template <class = detail::unspecified_month_disambiguator>
year_month_day_last operator+(const months& dm,
                              const year_month_day_last& ymdl) noexcept;

constexpr year_month_day_last operator+(const year_month_day_last& ymdl,
                                        const years& dy) noexcept;

constexpr year_month_day_last operator+(
    const years& dy, const year_month_day_last& ymdl) noexcept;

template <class = detail::unspecified_month_disambiguator>
year_month_day_last operator-(const year_month_day_last& ymdl,
                              const months& dm) noexcept;

constexpr year_month_day_last operator-(const year_month_day_last& ymdl,
                                        const years& dy) noexcept;

class year_month_day_last {
 public:
  constexpr year_month_day_last(const date::year& y,
                                const date::month_day_last& mdl) noexcept
      : _y(y), _mdl(mdl) {}

  template <class = detail::unspecified_month_disambiguator>
  year_month_day_last& operator+=(const months& m) noexcept {
    *this = *this + m;
    return *this;
  }

  template <class = detail::unspecified_month_disambiguator>
  year_month_day_last& operator-=(const months& m) noexcept {
    *this = *this - m;
    return *this;
  }

  year_month_day_last& operator+=(const years& y) noexcept {
    *this = *this + y;
    return *this;
  }

  year_month_day_last& operator-=(const years& y) noexcept {
    *this = *this - y;
    return *this;
  }

  constexpr date::year year() const noexcept { return _y; }
  constexpr date::month month() const noexcept { return _mdl.month(); }
  constexpr date::month_day_last month_day_last() const noexcept {
    return _mdl;
  }
  constexpr date::day day() const noexcept {
    constexpr const date::day d[] = {
        date::day(31), date::day(28), date::day(31), date::day(30),
        date::day(31), date::day(30), date::day(31), date::day(31),
        date::day(30), date::day(31), date::day(30), date::day(31)};
    return (month() != February || !_y.is_leap()) && _mdl.ok()
               ? d[static_cast<unsigned>(month()) - 1]
               : date::day{29};
  }

  operator sys_days() const noexcept {
    return sys_days(year() / month() / day());
  }

  explicit operator local_days() const noexcept {
    return local_days(year() / month() / day());
  }

  constexpr bool ok() const noexcept { return _y.ok() && _mdl.ok(); }

 private:
  date::year _y;
  date::month_day_last _mdl;
};

constexpr bool operator==(const year_month_day_last& x,
                          const year_month_day_last& y) noexcept {
  return x.year() == y.year() && x.month_day_last() == y.month_day_last();
}

constexpr bool operator!=(const year_month_day_last& x,
                          const year_month_day_last& y) noexcept {
  return !(x == y);
}

constexpr bool operator<(const year_month_day_last& x,
                         const year_month_day_last& y) noexcept {
  return x.year() < y.year()
             ? true
             : (x.year() > y.year()
                    ? false
                    : (x.month_day_last() < y.month_day_last()));
}

constexpr bool operator>(const year_month_day_last& x,
                         const year_month_day_last& y) noexcept {
  return y < x;
}

constexpr bool operator<=(const year_month_day_last& x,
                          const year_month_day_last& y) noexcept {
  return !(y < x);
}

constexpr bool operator>=(const year_month_day_last& x,
                          const year_month_day_last& y) noexcept {
  return !(x < y);
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const year_month_day_last& ymdl) {
  low_level_fmt(os, ymdl.year()) << '/';
  return low_level_fmt(os, ymdl.month_day_last());
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const year_month_day_last& ymdl) {
  detail::low_level_fmt(os, ymdl);
  if (!ymdl.ok()) os << " is not a valid year_month_day_last";
  return os;
}

template <class>
year_month_day_last operator+(const year_month_day_last& ymdl,
                              const months& dm) noexcept {
  return (ymdl.year() / ymdl.month() + dm) / last;
}

template <class>
year_month_day_last operator+(const months& dm,
                              const year_month_day_last& ymdl) noexcept {
  return ymdl + dm;
}

constexpr year_month_day_last operator+(const year_month_day_last& ymdl,
                                        const years& dy) noexcept {
  return {ymdl.year() + dy, ymdl.month_day_last()};
}

constexpr year_month_day_last operator+(
    const years& dy, const year_month_day_last& ymdl) noexcept {
  return ymdl + dy;
}

template <class>
year_month_day_last operator-(const year_month_day_last& ymdl,
                              const months& dm) noexcept {
  return ymdl + -dm;
}

constexpr year_month_day_last operator-(const year_month_day_last& ymdl,
                                        const years& dy) noexcept {
  return ymdl + -dy;
}

constexpr year_month_day_last operator/(const year_month& ym,
                                        last_spec) noexcept {
  return {ym.year(), month_day_last{ym.month()}};
}

constexpr year_month_day_last operator/(const year& y,
                                        const month_day_last& mdl) noexcept {
  return {y, mdl};
}

constexpr year_month_day_last operator/(int y,
                                        const month_day_last& mdl) noexcept {
  return year(y) / mdl;
}

constexpr year_month_day_last operator/(const month_day_last& mdl,
                                        const year& y) noexcept {
  return y / mdl;
}

constexpr year_month_day_last operator/(const month_day_last& mdl,
                                        int y) noexcept {
  return year(y) / mdl;
}

inline constexpr bool year_month_day::ok() const noexcept {
  if (!(_y.ok() && _m.ok())) return false;
  return date::day{1} <= _d && _d <= (_y / _m / last).day();
}

inline year_month_day::year_month_day(const year_month_day_last& ymdl) noexcept
    : _y(ymdl.year()), _m(ymdl.month()), _d(ymdl.day()) {}

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_YEAR_MONTH_DAY_HPP__