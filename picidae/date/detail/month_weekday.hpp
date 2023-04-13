#ifndef PICIDAE_DATE_DETAIL_MONTH_WEEKDAY_HPP__
#define PICIDAE_DATE_DETAIL_MONTH_WEEKDAY_HPP__

#include <sstream>

#include "picidae/date/detail/common.hpp"
#include "picidae/date/detail/month.hpp"
#include "picidae/date/detail/operator.hpp"
#include "picidae/date/detail/weekday.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

class month_weekday {
 public:
  constexpr month_weekday(const date::month& m,
                          const date::weekday_indexed& wdi) noexcept
      : _m(m), _wdi(wdi) {}

  constexpr date::month month() const noexcept { return _m; }

  constexpr date::weekday_indexed weekday_indexed() const noexcept {
    return _wdi;
  }

  constexpr bool ok() const noexcept { return _m.ok() && _wdi.ok(); }

 private:
  date::month _m;
  date::weekday_indexed _wdi;
};

constexpr month_weekday operator/(const month& m,
                                  const weekday_indexed& wdi) noexcept {
  return {m, wdi};
}

constexpr month_weekday operator/(int m, const weekday_indexed& wdi) noexcept {
  return month(static_cast<unsigned>(m)) / wdi;
}

constexpr month_weekday operator/(const weekday_indexed& wdi,
                                  const month& m) noexcept {
  return m / wdi;
}

constexpr month_weekday operator/(const weekday_indexed& wdi, int m) noexcept {
  return m / wdi;
}

constexpr bool operator==(const month_weekday& x,
                          const month_weekday& y) noexcept {
  return x.month() == y.month() && x.weekday_indexed() == y.weekday_indexed();
}

constexpr bool operator!=(const month_weekday& x,
                          const month_weekday& y) noexcept {
  return !(x == y);
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const month_weekday& mwd) {
  low_level_fmt(os, mwd.month()) << '/';
  return low_level_fmt(os, mwd.weekday_indexed());
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const month_weekday& mwd) {
  detail::low_level_fmt(os, mwd);
  if (!mwd.ok()) os << " is not a valid month_weekday";
  return os;
}

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_MONTH_WEEKDAY_HPP__