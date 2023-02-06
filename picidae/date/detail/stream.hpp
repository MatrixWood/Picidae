#ifndef PICIDAE_DATE_STREAM_HPP__
#define PICIDAE_DATE_STREAM_HPP__

#include <ostream>
#include <sstream>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

PICIDAE_NAMESPACE_BEGIN(stream)

template <class CharT, class Traits = std::char_traits<CharT>>
class save_istream {
 protected:
  std::basic_ios<CharT, Traits>& is_;
  CharT fill_;
  std::ios::fmtflags flags_;
  std::streamsize precision_;
  std::streamsize width_;
  std::basic_ostream<CharT, Traits>* tie_;
  std::locale loc_;

 public:
  ~save_istream() {
    is_.fill(fill_);
    is_.flags(flags_);
    is_.precision(precision_);
    is_.width(width_);
    is_.imbue(loc_);
    is_.tie(tie_);
  }

  save_istream(const save_istream&) = delete;
  save_istream& operator=(const save_istream&) = delete;

  explicit save_istream(std::basic_ios<CharT, Traits>& is)
      : is_(is),
        fill_(is.fill()),
        flags_(is.flags()),
        precision_(is.precision()),
        width_(is.width(0)),
        tie_(is.tie(nullptr)),
        loc_(is.getloc()) {
    if (tie_ != nullptr) tie_->flush();
  }
};

template <class CharT, class Traits = std::char_traits<CharT>>
class save_ostream : private save_istream<CharT, Traits> {
 public:
  ~save_ostream() {
    if ((this->flags_ & std::ios::unitbuf) && std::uncaught_exceptions() &&
        this->is_.good())
      this->is_.rdbuf()->pubsync();
  }

  save_ostream(const save_ostream&) = delete;
  save_ostream& operator=(const save_ostream&) = delete;

  explicit save_ostream(std::basic_ios<CharT, Traits>& os)
      : save_istream<CharT, Traits>(os) {}
};

PICIDAE_NAMESPACE_END(stream)

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_STREAM_HPP__