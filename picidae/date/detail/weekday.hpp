#ifndef PICIDAE_DATE_DETAIL_WEEKDAY_HPP__
#define PICIDAE_DATE_DETAIL_WEEKDAY_HPP__

#include <sstream>

#include "picidae/date/detail/common.hpp"
#include "picidae/date/detail/operator.hpp"
#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

class weekday;

constexpr bool operator==(const weekday& x, const weekday& y) noexcept;
constexpr bool operator!=(const weekday& x, const weekday& y) noexcept;

weekday operator+(const weekday& x, const days& y) noexcept;
weekday operator+(const days& x, const weekday& y) noexcept;
weekday operator-(const weekday& x, const days& y) noexcept;
days operator-(const weekday& x, const weekday& y) noexcept;

class weekday {
 public:
  weekday() = default;
  inline explicit constexpr weekday(unsigned wd) noexcept
      : _wd(static_cast<decltype(_wd)>(wd != 7 ? wd : 0)) {}

  inline weekday& operator++() noexcept {
    *this += days{1};
    return *this;
  }

  inline weekday operator++(int) noexcept {
    auto tmp(*this);
    ++(*this);
    return tmp;
  }

  inline weekday& operator--() noexcept {
    *this -= days{1};
    return *this;
  }

  inline weekday operator--(int) noexcept {
    auto tmp(*this);
    --(*this);
    return tmp;
  }

  inline weekday& operator+=(const days& d) noexcept {
    *this = *this + d;
    return *this;
  }

  inline weekday& operator-=(const days& d) noexcept {
    *this = *this - d;
    return *this;
  }

  inline constexpr unsigned c_encoding() const noexcept {
    return unsigned{_wd};
  }

  inline constexpr unsigned iso_encoding() const noexcept {
    return unsigned{((_wd == 0u) ? 7u : _wd)};
  }

  constexpr bool ok() const noexcept { return _wd <= 6; }

  constexpr weekday_indexed operator[](unsigned index) const noexcept;

  constexpr weekday_last operator[](last_spec) const noexcept;

 private:
  friend constexpr bool operator==(const weekday& x,
                                   const weekday& y) noexcept {
    return x._wd == y._wd;
  }

  friend days operator-(const weekday& x, const weekday& y) noexcept {
    const auto wdu = x._wd - y._wd;
    const auto wk = (wdu >= 0 ? wdu : wdu - 6) / 7;
    return days{wdu - wk * 7};
  }

  friend weekday operator+(const weekday& x, const days& y) noexcept {
    const auto wdu =
        static_cast<long long>(static_cast<unsigned>(x._wd)) + y.count();
    const auto wk = (wdu >= 0 ? wdu : wdu - 6) / 7;
    return weekday{static_cast<unsigned>(wdu - wk * 7)};
  }

 private:
  unsigned char _wd;
  friend class weekday_indexed;
};

inline constexpr bool operator!=(const weekday& x, const weekday& y) noexcept {
  return !(x == y);
}

inline weekday operator+(const days& x, const weekday& y) noexcept {
  return y + x;
}

inline weekday operator-(const weekday& x, const days& y) noexcept {
  return x + -y;
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const weekday& wd) {
  if (wd.ok()) {
    CharT fmt[] = {'%', 'a', 0};
    // os << format(fmt, wd);
    os << wd.c_encoding();
  } else
    os << wd.c_encoding();
  return os;
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const weekday& wd) {
  detail::low_level_fmt(os, wd);
  if (!wd.ok()) os << " is not a valid weekday";
  return os;
}

constexpr const weekday sun{0u};
constexpr const weekday mon{1u};
constexpr const weekday tue{2u};
constexpr const weekday wed{3u};
constexpr const weekday thu{4u};
constexpr const weekday fri{5u};
constexpr const weekday sat{6u};

constexpr const weekday Monday{1};
constexpr const weekday Tuesday{2};
constexpr const weekday Wednesday{3};
constexpr const weekday Thursday{4};
constexpr const weekday Friday{5};
constexpr const weekday Saturday{6};
constexpr const weekday Sunday{7};

class weekday_indexed {
 public:
  weekday_indexed() = default;
  constexpr weekday_indexed(const weekday& wd, unsigned index) noexcept
      : _wd(static_cast<decltype(_wd)>(static_cast<unsigned>(wd._wd))),
        _index(static_cast<decltype(_index)>(index)) {}

  constexpr weekday weekday() const noexcept {
    return date::weekday{static_cast<unsigned>(_wd)};
  }

  constexpr unsigned index() const noexcept { return _index; }

  constexpr bool ok() const noexcept {
    return weekday().ok() && 1 <= _index && _index <= 5;
  }

 private:
  unsigned char _wd : 4;
  unsigned char _index : 4;
};

constexpr bool operator==(const weekday_indexed& x,
                          const weekday_indexed& y) noexcept {
  return x.weekday() == y.weekday() && x.index() == y.index();
}

constexpr bool operator!=(const weekday_indexed& x,
                          const weekday_indexed& y) noexcept {
  return !(x == y);
}

inline constexpr weekday_indexed weekday::operator[](
    unsigned index) const noexcept {
  return {*this, index};
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const weekday_indexed& wdi) {
  return low_level_fmt(os, wdi.weekday()) << '[' << wdi.index() << ']';
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const weekday_indexed& wdi) {
  detail::low_level_fmt(os, wdi);
  if (!wdi.ok()) os << " is not a valid weekday_indexed";
  return os;
}

class weekday_last {
 public:
  explicit constexpr weekday_last(const date::weekday& wd) noexcept : _wd(wd) {}

  constexpr date::weekday weekday() const noexcept { return _wd; }

  constexpr bool ok() const noexcept { return _wd.ok(); }

 private:
  date::weekday _wd;
};

constexpr bool operator==(const weekday_last& x,
                          const weekday_last& y) noexcept {
  return x.weekday() == y.weekday();
}

constexpr bool operator!=(const weekday_last& x,
                          const weekday_last& y) noexcept {
  return !(x == y);
}

inline constexpr weekday_last weekday::operator[](
    last_spec) const noexcept {
  return weekday_last{*this};
}

PICIDAE_NAMESPACE_BEGIN(detail)

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& low_level_fmt(
    std::basic_ostream<CharT, Traits>& os, const weekday_last& wdl) {
  return low_level_fmt(os, wdl.weekday()) << "[last]";
}

PICIDAE_NAMESPACE_END(detail)

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const weekday_last& wdl) {
  detail::low_level_fmt(os, wdl);
  if (!wdl.ok()) os << " is not a valid weekday_last";
  return os;
}

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_WEEKDAY_HPP__