#ifndef PICIDAE_DATE_DETAIL_DECL_HPP__
#define PICIDAE_DATE_DETAIL_DECL_HPP__

#include "picidae/date/detail/common.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

struct last_spec {
  explicit last_spec() = default;
};

constexpr const last_spec last{};

PICIDAE_NAMESPACE_BEGIN(detail)
struct unspecified_month_disambiguator {};
PICIDAE_NAMESPACE_END(detail)

class day;
class month;
class year;

class weekday;
class weekday_indexed;
class weekday_last;

class month_day;
class month_day_last;
class month_weekday;
class month_weekday_last;

class year_month;

class year_month_day;
class year_month_day_last;
class year_month_weekday;
class year_month_weekday_last;

// date composition operators

constexpr year_month operator/(const year& y, const month& m) noexcept;

constexpr year_month operator/(const year& y, int m) noexcept;

constexpr month_day operator/(const day& d, const month& m) noexcept;
constexpr month_day operator/(const day& d, int m) noexcept;
constexpr month_day operator/(const month& m, const day& d) noexcept;
constexpr month_day operator/(const month& m, int d) noexcept;
constexpr month_day operator/(int m, const day& d) noexcept;

constexpr month_day_last operator/(const month& m, last_spec) noexcept;
constexpr month_day_last operator/(int m, last_spec) noexcept;
constexpr month_day_last operator/(last_spec, const month& m) noexcept;
constexpr month_day_last operator/(last_spec, int m) noexcept;

constexpr month_weekday operator/(const month& m,
                                  const weekday_indexed& wdi) noexcept;
constexpr month_weekday operator/(int m, const weekday_indexed& wdi) noexcept;
constexpr month_weekday operator/(const weekday_indexed& wdi,
                                  const month& m) noexcept;
constexpr month_weekday operator/(const weekday_indexed& wdi, int m) noexcept;

constexpr month_weekday_last operator/(const month& m,
                                       const weekday_last& wdl) noexcept;
constexpr month_weekday_last operator/(int m, const weekday_last& wdl) noexcept;
constexpr month_weekday_last operator/(const weekday_last& wdl,
                                       const month& m) noexcept;
constexpr month_weekday_last operator/(const weekday_last& wdl, int m) noexcept;

constexpr year_month_day operator/(const year_month& ym, const day& d) noexcept;
constexpr year_month_day operator/(const year_month& ym, int d) noexcept;
constexpr year_month_day operator/(const year& y, const month_day& md) noexcept;
constexpr year_month_day operator/(int y, const month_day& md) noexcept;
constexpr year_month_day operator/(const month_day& md, const year& y) noexcept;
constexpr year_month_day operator/(const month_day& md, int y) noexcept;

constexpr year_month_day_last operator/(const year_month& ym,
                                        last_spec) noexcept;
constexpr year_month_day_last operator/(const year& y,
                                        const month_day_last& mdl) noexcept;
constexpr year_month_day_last operator/(int y,
                                        const month_day_last& mdl) noexcept;
constexpr year_month_day_last operator/(const month_day_last& mdl,
                                        const year& y) noexcept;
constexpr year_month_day_last operator/(const month_day_last& mdl,
                                        int y) noexcept;

constexpr year_month_weekday operator/(const year_month& ym,
                                       const weekday_indexed& wdi) noexcept;

constexpr year_month_weekday operator/(const year& y,
                                       const month_weekday& mwd) noexcept;

constexpr year_month_weekday operator/(int y,
                                       const month_weekday& mwd) noexcept;

constexpr year_month_weekday operator/(const month_weekday& mwd,
                                       const year& y) noexcept;

constexpr year_month_weekday operator/(const month_weekday& mwd,
                                       int y) noexcept;

constexpr year_month_weekday_last operator/(const year_month& ym,
                                            const weekday_last& wdl) noexcept;

constexpr year_month_weekday_last operator/(
    const year& y, const month_weekday_last& mwdl) noexcept;

constexpr year_month_weekday_last operator/(
    int y, const month_weekday_last& mwdl) noexcept;

constexpr year_month_weekday_last operator/(const month_weekday_last& mwdl,
                                            const year& y) noexcept;

constexpr year_month_weekday_last operator/(const month_weekday_last& mwdl,
                                            int y) noexcept;

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_DECL_HPP__