#ifndef PICIDAE_DATE_FORMAT_HPP__
#define PICIDAE_DATE_FORMAT_HPP__

#include <chrono>
#include <ostream>
#include <sstream>

#include "picidae/date/stream.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

PICIDAE_NAMESPACE_BEGIN(format)

template <class Duration>
struct fields
{
    year_month_day        ymd{nanyear/0/0};
    weekday               wd{8u};
    hh_mm_ss<Duration>    tod{};
    bool                  has_tod = false;

#if !defined(__clang__) && defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ <= 409)
    fields() : ymd{nanyear/0/0}, wd{8u}, tod{}, has_tod{false} {}
#else
    fields() = default;
#endif

    fields(year_month_day ymd_) : ymd(ymd_) {}
    fields(weekday wd_) : wd(wd_) {}
    fields(hh_mm_ss<Duration> tod_) : tod(tod_), has_tod(true) {}

    fields(year_month_day ymd_, weekday wd_) : ymd(ymd_), wd(wd_) {}
    fields(year_month_day ymd_, hh_mm_ss<Duration> tod_) : ymd(ymd_), tod(tod_),
                                                           has_tod(true) {}

    fields(weekday wd_, hh_mm_ss<Duration> tod_) : wd(wd_), tod(tod_), has_tod(true) {}

    fields(year_month_day ymd_, weekday wd_, hh_mm_ss<Duration> tod_)
        : ymd(ymd_)
        , wd(wd_)
        , tod(tod_)
        , has_tod(true)
        {}
};

template <class CharT, class Traits, class Duration>
std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt,
    const fields<Duration>& fds, const std::string* abbrev,
    const std::chrono::seconds* offset_sec) {
  using detail::extract_month;
  using detail::extract_weekday;
  using detail::get_units;
  using stream::save_ostream;
  using std::ios;
  using std::chrono::duration_cast;
  using std::chrono::hours;
  using std::chrono::minutes;
  using std::chrono::seconds;
  date::detail::save_ostream<CharT, Traits> ss(os);
  os.fill(' ');
  os.flags(std::ios::skipws | std::ios::dec);
  os.width(0);
  tm tm{};
  bool insert_negative =
      fds.has_tod && fds.tod.to_duration() < Duration::zero();
#if !ONLY_C_LOCALE
  auto& facet = std::use_facet<std::time_put<CharT>>(os.getloc());
#endif
  const CharT* command = nullptr;
  CharT modified = CharT{};
  for (; *fmt; ++fmt) {
    switch (*fmt) {
      case 'a':
      case 'A':
        if (command) {
          if (modified == CharT{}) {
            tm.tm_wday = static_cast<int>(extract_weekday(os, fds));
            if (os.fail()) return os;
#if !ONLY_C_LOCALE
            const CharT f[] = {'%', *fmt};
            facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
#else   // ONLY_C_LOCALE
            os << weekday_names().first[tm.tm_wday + 7 * (*fmt == 'a')];
#endif  // ONLY_C_LOCALE
          } else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'b':
      case 'B':
      case 'h':
        if (command) {
          if (modified == CharT{}) {
            tm.tm_mon = static_cast<int>(extract_month(os, fds)) - 1;
#if !ONLY_C_LOCALE
            const CharT f[] = {'%', *fmt};
            facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
#else   // ONLY_C_LOCALE
            os << month_names().first[tm.tm_mon + 12 * (*fmt != 'B')];
#endif  // ONLY_C_LOCALE
          } else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'c':
      case 'x':
        if (command) {
          if (modified == CharT{'O'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.ymd.ok()) os.setstate(std::ios::failbit);
            if (*fmt == 'c' && !fds.has_tod) os.setstate(std::ios::failbit);
#if !ONLY_C_LOCALE
            tm = std::tm{};
            auto const& ymd = fds.ymd;
            auto ld = local_days(ymd);
            if (*fmt == 'c') {
              tm.tm_sec = static_cast<int>(fds.tod.seconds().count());
              tm.tm_min = static_cast<int>(fds.tod.minutes().count());
              tm.tm_hour = static_cast<int>(fds.tod.hours().count());
            }
            tm.tm_mday = static_cast<int>(static_cast<unsigned>(ymd.day()));
            tm.tm_mon = static_cast<int>(extract_month(os, fds) - 1);
            tm.tm_year = static_cast<int>(ymd.year()) - 1900;
            tm.tm_wday = static_cast<int>(extract_weekday(os, fds));
            if (os.fail()) return os;
            tm.tm_yday =
                static_cast<int>((ld - local_days(ymd.year() / 1 / 1)).count());
            CharT f[3] = {'%'};
            auto fe = std::begin(f) + 1;
            if (modified == CharT{'E'}) *fe++ = modified;
            *fe++ = *fmt;
            facet.put(os, os, os.fill(), &tm, std::begin(f), fe);
#else   // ONLY_C_LOCALE
            if (*fmt == 'c') {
              auto wd = static_cast<int>(extract_weekday(os, fds));
              os << weekday_names().first[static_cast<unsigned>(wd) + 7] << ' ';
              os << month_names().first[extract_month(os, fds) - 1 + 12] << ' ';
              auto d = static_cast<int>(static_cast<unsigned>(fds.ymd.day()));
              if (d < 10) os << ' ';
              os << d << ' '
                 << make_time(duration_cast<seconds>(fds.tod.to_duration()))
                 << ' ' << fds.ymd.year();

            } else  // *fmt == 'x'
            {
              auto const& ymd = fds.ymd;
              save_ostream<CharT, Traits> _(os);
              os.fill('0');
              os.flags(std::ios::dec | std::ios::right);
              os.width(2);
              os << static_cast<unsigned>(ymd.month()) << CharT{'/'};
              os.width(2);
              os << static_cast<unsigned>(ymd.day()) << CharT{'/'};
              os.width(2);
              os << static_cast<int>(ymd.year()) % 100;
            }
#endif  // ONLY_C_LOCALE
          }
          command = nullptr;
          modified = CharT{};
        } else
          os << *fmt;
        break;
      case 'C':
        if (command) {
          if (modified == CharT{'O'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.ymd.year().ok()) os.setstate(std::ios::failbit);
            auto y = static_cast<int>(fds.ymd.year());
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              save_ostream<CharT, Traits> _(os);
              os.fill('0');
              os.flags(std::ios::dec | std::ios::right);
              if (y >= 0) {
                os.width(2);
                os << y / 100;
              } else {
                os << CharT{'-'};
                os.width(2);
                os << -(y - 99) / 100;
              }
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'E'}) {
              tm.tm_year = y - 1900;
              CharT f[3] = {'%', 'E', 'C'};
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          command = nullptr;
          modified = CharT{};
        } else
          os << *fmt;
        break;
      case 'd':
      case 'e':
        if (command) {
          if (modified == CharT{'E'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.ymd.day().ok()) os.setstate(std::ios::failbit);
            auto d = static_cast<int>(static_cast<unsigned>(fds.ymd.day()));
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              save_ostream<CharT, Traits> _(os);
              if (*fmt == CharT{'d'})
                os.fill('0');
              else
                os.fill(' ');
              os.flags(std::ios::dec | std::ios::right);
              os.width(2);
              os << d;
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'O'}) {
              tm.tm_mday = d;
              CharT f[3] = {'%', 'O', *fmt};
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          command = nullptr;
          modified = CharT{};
        } else
          os << *fmt;
        break;
      case 'D':
        if (command) {
          if (modified == CharT{}) {
            if (!fds.ymd.ok()) os.setstate(std::ios::failbit);
            auto const& ymd = fds.ymd;
            save_ostream<CharT, Traits> _(os);
            os.fill('0');
            os.flags(std::ios::dec | std::ios::right);
            os.width(2);
            os << static_cast<unsigned>(ymd.month()) << CharT{'/'};
            os.width(2);
            os << static_cast<unsigned>(ymd.day()) << CharT{'/'};
            os.width(2);
            os << static_cast<int>(ymd.year()) % 100;
          } else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'F':
        if (command) {
          if (modified == CharT{}) {
            if (!fds.ymd.ok()) os.setstate(std::ios::failbit);
            auto const& ymd = fds.ymd;
            save_ostream<CharT, Traits> _(os);
            os.imbue(std::locale::classic());
            os.fill('0');
            os.flags(std::ios::dec | std::ios::right);
            os.width(4);
            os << static_cast<int>(ymd.year()) << CharT{'-'};
            os.width(2);
            os << static_cast<unsigned>(ymd.month()) << CharT{'-'};
            os.width(2);
            os << static_cast<unsigned>(ymd.day());
          } else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'g':
      case 'G':
        if (command) {
          if (modified == CharT{}) {
            if (!fds.ymd.ok()) os.setstate(std::ios::failbit);
            auto ld = local_days(fds.ymd);
            auto y = year_month_day{ld + days{3}}.year();
            auto start =
                local_days((y - years{1}) / December / Thursday[last]) +
                (Monday - Thursday);
            if (ld < start) --y;
            if (*fmt == CharT{'G'})
              os << y;
            else {
              save_ostream<CharT, Traits> _(os);
              os.fill('0');
              os.flags(std::ios::dec | std::ios::right);
              os.width(2);
              os << std::abs(static_cast<int>(y)) % 100;
            }
          } else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'H':
      case 'I':
        if (command) {
          if (modified == CharT{'E'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.has_tod) os.setstate(std::ios::failbit);
            if (insert_negative) {
              os << '-';
              insert_negative = false;
            }
            auto hms = fds.tod;
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              auto h =
                  *fmt == CharT{'I'} ? date::make12(hms.hours()) : hms.hours();
              if (h < hours{10}) os << CharT{'0'};
              os << h.count();
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'O'}) {
              const CharT f[] = {'%', modified, *fmt};
              tm.tm_hour = static_cast<int>(hms.hours().count());
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'j':
        if (command) {
          if (modified == CharT{}) {
            if (fds.ymd.ok() || fds.has_tod) {
              days doy;
              if (fds.ymd.ok()) {
                auto ld = local_days(fds.ymd);
                auto y = fds.ymd.year();
                doy = ld - local_days(y / January / 1) + days{1};
              } else {
                doy = duration_cast<days>(fds.tod.to_duration());
              }
              save_ostream<CharT, Traits> _(os);
              os.fill('0');
              os.flags(std::ios::dec | std::ios::right);
              os.width(3);
              os << doy.count();
            } else {
              os.setstate(std::ios::failbit);
            }
          } else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'm':
        if (command) {
          if (modified == CharT{'E'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.ymd.month().ok()) os.setstate(std::ios::failbit);
            auto m = static_cast<unsigned>(fds.ymd.month());
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              if (m < 10) os << CharT{'0'};
              os << m;
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'O'}) {
              const CharT f[] = {'%', modified, *fmt};
              tm.tm_mon = static_cast<int>(m - 1);
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'M':
        if (command) {
          if (modified == CharT{'E'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.has_tod) os.setstate(std::ios::failbit);
            if (insert_negative) {
              os << '-';
              insert_negative = false;
            }
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              if (fds.tod.minutes() < minutes{10}) os << CharT{'0'};
              os << fds.tod.minutes().count();
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'O'}) {
              const CharT f[] = {'%', modified, *fmt};
              tm.tm_min = static_cast<int>(fds.tod.minutes().count());
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'n':
        if (command) {
          if (modified == CharT{})
            os << CharT{'\n'};
          else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'p':
        if (command) {
          if (modified == CharT{}) {
            if (!fds.has_tod) os.setstate(std::ios::failbit);
#if !ONLY_C_LOCALE
            const CharT f[] = {'%', *fmt};
            tm.tm_hour = static_cast<int>(fds.tod.hours().count());
            facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
#else
            if (date::is_am(fds.tod.hours()))
              os << ampm_names().first[0];
            else
              os << ampm_names().first[1];
#endif
          } else {
            os << CharT{'%'} << modified << *fmt;
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'Q':
      case 'q':
        if (command) {
          if (modified == CharT{}) {
            if (!fds.has_tod) os.setstate(std::ios::failbit);
            auto d = fds.tod.to_duration();
            if (*fmt == 'q')
              os << get_units<CharT>(typename decltype(d)::period::type{});
            else
              os << d.count();
          } else {
            os << CharT{'%'} << modified << *fmt;
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'r':
        if (command) {
          if (modified == CharT{}) {
            if (!fds.has_tod) os.setstate(std::ios::failbit);
#if !ONLY_C_LOCALE
            const CharT f[] = {'%', *fmt};
            tm.tm_hour = static_cast<int>(fds.tod.hours().count());
            tm.tm_min = static_cast<int>(fds.tod.minutes().count());
            tm.tm_sec = static_cast<int>(fds.tod.seconds().count());
            facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
#else
            hh_mm_ss<seconds> tod(
                duration_cast<seconds>(fds.tod.to_duration()));
            save_ostream<CharT, Traits> _(os);
            os.fill('0');
            os.width(2);
            os << date::make12(tod.hours()).count() << CharT{':'};
            os.width(2);
            os << tod.minutes().count() << CharT{':'};
            os.width(2);
            os << tod.seconds().count() << CharT{' '};
            if (date::is_am(tod.hours()))
              os << ampm_names().first[0];
            else
              os << ampm_names().first[1];
#endif
          } else {
            os << CharT{'%'} << modified << *fmt;
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'R':
        if (command) {
          if (modified == CharT{}) {
            if (!fds.has_tod) os.setstate(std::ios::failbit);
            if (fds.tod.hours() < hours{10}) os << CharT{'0'};
            os << fds.tod.hours().count() << CharT{':'};
            if (fds.tod.minutes() < minutes{10}) os << CharT{'0'};
            os << fds.tod.minutes().count();
          } else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'S':
        if (command) {
          if (modified == CharT{'E'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.has_tod) os.setstate(std::ios::failbit);
            if (insert_negative) {
              os << '-';
              insert_negative = false;
            }
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              os << fds.tod.s_;
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'O'}) {
              const CharT f[] = {'%', modified, *fmt};
              tm.tm_sec = static_cast<int>(fds.tod.s_.seconds().count());
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 't':
        if (command) {
          if (modified == CharT{})
            os << CharT{'\t'};
          else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'T':
        if (command) {
          if (modified == CharT{}) {
            if (!fds.has_tod) os.setstate(std::ios::failbit);
            os << fds.tod;
          } else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'u':
        if (command) {
          if (modified == CharT{'E'})
            os << CharT{'%'} << modified << *fmt;
          else {
            auto wd = extract_weekday(os, fds);
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              os << (wd != 0 ? wd : 7u);
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'O'}) {
              const CharT f[] = {'%', modified, *fmt};
              tm.tm_wday = static_cast<int>(wd);
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'U':
        if (command) {
          if (modified == CharT{'E'})
            os << CharT{'%'} << modified << *fmt;
          else {
            auto const& ymd = fds.ymd;
            if (!ymd.ok()) os.setstate(std::ios::failbit);
            auto ld = local_days(ymd);
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              auto st = local_days(Sunday[1] / January / ymd.year());
              if (ld < st)
                os << CharT{'0'} << CharT{'0'};
              else {
                auto wn = duration_cast<weeks>(ld - st).count() + 1;
                if (wn < 10) os << CharT{'0'};
                os << wn;
              }
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'O'}) {
              const CharT f[] = {'%', modified, *fmt};
              tm.tm_year = static_cast<int>(ymd.year()) - 1900;
              tm.tm_wday = static_cast<int>(extract_weekday(os, fds));
              if (os.fail()) return os;
              tm.tm_yday = static_cast<int>(
                  (ld - local_days(ymd.year() / 1 / 1)).count());
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'V':
        if (command) {
          if (modified == CharT{'E'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.ymd.ok()) os.setstate(std::ios::failbit);
            auto ld = local_days(fds.ymd);
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              auto y = year_month_day{ld + days{3}}.year();
              auto st = local_days((y - years{1}) / 12 / Thursday[last]) +
                        (Monday - Thursday);
              if (ld < st) {
                --y;
                st = local_days((y - years{1}) / 12 / Thursday[last]) +
                     (Monday - Thursday);
              }
              auto wn = duration_cast<weeks>(ld - st).count() + 1;
              if (wn < 10) os << CharT{'0'};
              os << wn;
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'O'}) {
              const CharT f[] = {'%', modified, *fmt};
              auto const& ymd = fds.ymd;
              tm.tm_year = static_cast<int>(ymd.year()) - 1900;
              tm.tm_wday = static_cast<int>(extract_weekday(os, fds));
              if (os.fail()) return os;
              tm.tm_yday = static_cast<int>(
                  (ld - local_days(ymd.year() / 1 / 1)).count());
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'w':
        if (command) {
          auto wd = extract_weekday(os, fds);
          if (os.fail()) return os;
#if !ONLY_C_LOCALE
          if (modified == CharT{})
#else
          if (modified != CharT{'E'})
#endif
          {
            os << wd;
          }
#if !ONLY_C_LOCALE
          else if (modified == CharT{'O'}) {
            const CharT f[] = {'%', modified, *fmt};
            tm.tm_wday = static_cast<int>(wd);
            facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
          }
#endif
          else {
            os << CharT{'%'} << modified << *fmt;
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'W':
        if (command) {
          if (modified == CharT{'E'})
            os << CharT{'%'} << modified << *fmt;
          else {
            auto const& ymd = fds.ymd;
            if (!ymd.ok()) os.setstate(std::ios::failbit);
            auto ld = local_days(ymd);
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              auto st = local_days(Monday[1] / January / ymd.year());
              if (ld < st)
                os << CharT{'0'} << CharT{'0'};
              else {
                auto wn = duration_cast<weeks>(ld - st).count() + 1;
                if (wn < 10) os << CharT{'0'};
                os << wn;
              }
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'O'}) {
              const CharT f[] = {'%', modified, *fmt};
              tm.tm_year = static_cast<int>(ymd.year()) - 1900;
              tm.tm_wday = static_cast<int>(extract_weekday(os, fds));
              if (os.fail()) return os;
              tm.tm_yday = static_cast<int>(
                  (ld - local_days(ymd.year() / 1 / 1)).count());
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'X':
        if (command) {
          if (modified == CharT{'O'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.has_tod) os.setstate(std::ios::failbit);
#if !ONLY_C_LOCALE
            tm = std::tm{};
            tm.tm_sec = static_cast<int>(fds.tod.seconds().count());
            tm.tm_min = static_cast<int>(fds.tod.minutes().count());
            tm.tm_hour = static_cast<int>(fds.tod.hours().count());
            CharT f[3] = {'%'};
            auto fe = std::begin(f) + 1;
            if (modified == CharT{'E'}) *fe++ = modified;
            *fe++ = *fmt;
            facet.put(os, os, os.fill(), &tm, std::begin(f), fe);
#else
            os << fds.tod;
#endif
          }
          command = nullptr;
          modified = CharT{};
        } else
          os << *fmt;
        break;
      case 'y':
        if (command) {
          if (!fds.ymd.year().ok()) os.setstate(std::ios::failbit);
          auto y = static_cast<int>(fds.ymd.year());
#if !ONLY_C_LOCALE
          if (modified == CharT{}) {
#endif
            y = std::abs(y) % 100;
            if (y < 10) os << CharT{'0'};
            os << y;
#if !ONLY_C_LOCALE
          } else {
            const CharT f[] = {'%', modified, *fmt};
            tm.tm_year = y - 1900;
            facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
          }
#endif
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'Y':
        if (command) {
          if (modified == CharT{'O'})
            os << CharT{'%'} << modified << *fmt;
          else {
            if (!fds.ymd.year().ok()) os.setstate(std::ios::failbit);
            auto y = fds.ymd.year();
#if !ONLY_C_LOCALE
            if (modified == CharT{})
#endif
            {
              save_ostream<CharT, Traits> _(os);
              os.imbue(std::locale::classic());
              os << y;
            }
#if !ONLY_C_LOCALE
            else if (modified == CharT{'E'}) {
              const CharT f[] = {'%', modified, *fmt};
              tm.tm_year = static_cast<int>(y) - 1900;
              facet.put(os, os, os.fill(), &tm, std::begin(f), std::end(f));
            }
#endif
          }
          modified = CharT{};
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'z':
        if (command) {
          if (offset_sec == nullptr) {
            // Can not format %z with unknown offset
            os.setstate(ios::failbit);
            return os;
          }
          auto m = duration_cast<minutes>(*offset_sec);
          auto neg = m < minutes{0};
          m = date::abs(m);
          auto h = duration_cast<hours>(m);
          m -= h;
          if (neg)
            os << CharT{'-'};
          else
            os << CharT{'+'};
          if (h < hours{10}) os << CharT{'0'};
          os << h.count();
          if (modified != CharT{}) os << CharT{':'};
          if (m < minutes{10}) os << CharT{'0'};
          os << m.count();
          command = nullptr;
          modified = CharT{};
        } else
          os << *fmt;
        break;
      case 'Z':
        if (command) {
          if (modified == CharT{}) {
            if (abbrev == nullptr) {
              // Can not format %Z with unknown time_zone
              os.setstate(ios::failbit);
              return os;
            }
            for (auto c : *abbrev) os << CharT(c);
          } else {
            os << CharT{'%'} << modified << *fmt;
            modified = CharT{};
          }
          command = nullptr;
        } else
          os << *fmt;
        break;
      case 'E':
      case 'O':
        if (command) {
          if (modified == CharT{}) {
            modified = *fmt;
          } else {
            os << CharT{'%'} << modified << *fmt;
            command = nullptr;
            modified = CharT{};
          }
        } else
          os << *fmt;
        break;
      case '%':
        if (command) {
          if (modified == CharT{}) {
            os << CharT{'%'};
            command = nullptr;
          } else {
            os << CharT{'%'} << modified << CharT{'%'};
            command = nullptr;
            modified = CharT{};
          }
        } else
          command = fmt;
        break;
      default:
        if (command) {
          os << CharT{'%'};
          command = nullptr;
        }
        if (modified != CharT{}) {
          os << modified;
          modified = CharT{};
        }
        os << *fmt;
        break;
    }
  }
  if (command) os << CharT{'%'};
  if (modified != CharT{}) os << modified;
  return os;
}

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt, const year& y) {
  using CT = std::chrono::seconds;
  fields<CT> fds{y / 0 / 0};
  return to_stream(os, fmt, fds);
}

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt, const month& m) {
  using CT = std::chrono::seconds;
  fields<CT> fds{m / 0 / nanyear};
  return to_stream(os, fmt, fds);
}

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt, const day& d) {
  using CT = std::chrono::seconds;
  fields<CT> fds{d / 0 / nanyear};
  return to_stream(os, fmt, fds);
}

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt,
    const weekday& wd) {
  using CT = std::chrono::seconds;
  fields<CT> fds{wd};
  return to_stream(os, fmt, fds);
}

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt,
    const year_month& ym) {
  using CT = std::chrono::seconds;
  fields<CT> fds{ym / 0};
  return to_stream(os, fmt, fds);
}

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt,
    const month_day& md) {
  using CT = std::chrono::seconds;
  fields<CT> fds{md / nanyear};
  return to_stream(os, fmt, fds);
}

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt,
    const year_month_day& ymd) {
  using CT = std::chrono::seconds;
  fields<CT> fds{ymd};
  return to_stream(os, fmt, fds);
}

template <class CharT, class Traits, class Rep, class Period>
inline std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt,
    const std::chrono::duration<Rep, Period>& d) {
  using Duration = std::chrono::duration<Rep, Period>;
  using CT = typename std::common_type<Duration, std::chrono::seconds>::type;
  fields<CT> fds{hh_mm_ss<CT>{d}};
  return to_stream(os, fmt, fds);
}

template <class CharT, class Traits, class Duration>
std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt,
    const local_time<Duration>& tp, const std::string* abbrev = nullptr,
    const std::chrono::seconds* offset_sec = nullptr) {
  using CT = typename std::common_type<Duration, std::chrono::seconds>::type;
  auto ld = std::chrono::time_point_cast<days>(tp);
  fields<CT> fds;
  if (ld <= tp)
    fds = fields<CT>{year_month_day{ld}, hh_mm_ss<CT>{tp - local_seconds{ld}}};
  else
    fds = fields<CT>{year_month_day{ld - days{1}},
                     hh_mm_ss<CT>{days{1} - (local_seconds{ld} - tp)}};
  return to_stream(os, fmt, fds, abbrev, offset_sec);
}

template <class CharT, class Traits, class Duration>
std::basic_ostream<CharT, Traits>& to_stream(
    std::basic_ostream<CharT, Traits>& os, const CharT* fmt,
    const sys_time<Duration>& tp) {
  using std::chrono::seconds;
  using CT = typename std::common_type<Duration, seconds>::type;
  const std::string abbrev("UTC");
  CONSTDATA seconds offset{0};
  auto sd = std::chrono::time_point_cast<days>(tp);
  fields<CT> fds;
  if (sd <= tp)
    fds = fields<CT>{year_month_day{sd}, hh_mm_ss<CT>{tp - sys_seconds{sd}}};
  else
    fds = fields<CT>{year_month_day{sd - days{1}},
                     hh_mm_ss<CT>{days{1} - (sys_seconds{sd} - tp)}};
  return to_stream(os, fmt, fds, &abbrev, &offset);
}

template <class CharT, class Streamable>
auto format(const std::locale& loc, const CharT* fmt, const Streamable& tp)
    -> decltype(to_stream(std::declval<std::basic_ostream<CharT>&>(), fmt, tp),
                std::basic_string<CharT>{}) {
  std::basic_ostringstream<CharT> os;
  os.exceptions(std::ios::failbit | std::ios::badbit);
  os.imbue(loc);
  to_stream(os, fmt, tp);
  return os.str();
}

template <class CharT, class Streamable>
auto format(const CharT* fmt, const Streamable& tp)
    -> decltype(to_stream(std::declval<std::basic_ostream<CharT>&>(), fmt, tp),
                std::basic_string<CharT>{}) {
  std::basic_ostringstream<CharT> os;
  os.exceptions(std::ios::failbit | std::ios::badbit);
  to_stream(os, fmt, tp);
  return os.str();
}

template <class CharT, class Traits, class Alloc, class Streamable>
auto format(const std::locale& loc,
            const std::basic_string<CharT, Traits, Alloc>& fmt,
            const Streamable& tp)
    -> decltype(to_stream(std::declval<std::basic_ostream<CharT, Traits>&>(),
                          fmt.c_str(), tp),
                std::basic_string<CharT, Traits, Alloc>{}) {
  std::basic_ostringstream<CharT, Traits, Alloc> os;
  os.exceptions(std::ios::failbit | std::ios::badbit);
  os.imbue(loc);
  to_stream(os, fmt.c_str(), tp);
  return os.str();
}

template <class CharT, class Traits, class Alloc, class Streamable>
auto format(const std::basic_string<CharT, Traits, Alloc>& fmt,
            const Streamable& tp)
    -> decltype(to_stream(std::declval<std::basic_ostream<CharT, Traits>&>(),
                          fmt.c_str(), tp),
                std::basic_string<CharT, Traits, Alloc>{}) {
  std::basic_ostringstream<CharT, Traits, Alloc> os;
  os.exceptions(std::ios::failbit | std::ios::badbit);
  to_stream(os, fmt.c_str(), tp);
  return os.str();
}

PICIDAE_NAMESPACE_END(format)

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_FORMAT_HPP__