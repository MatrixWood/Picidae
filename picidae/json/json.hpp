#ifndef PICIDAE_JSON_JSON_HPP__
#define PICIDAE_JSON_JSON_HPP__

#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(json)

class Json {
 public:
  enum class Class {
    Null = 0,
    Object,
    Array,
    String,
    Floating,
    Integral,
    Boolean
  };

 private:
  using Data =
      std::variant<std::nullptr_t, std::map<std::string, Json>,
                   std::vector<Json>, std::string, double, std::int64_t, bool>;

  struct Internal {
    Internal(std::nullptr_t) : _d(nullptr) {}
    Internal() : _d(nullptr) {}
    Internal(Class c) : _d(make_type(c)) {}
    template <typename T>
    Internal(T t) : _d(std::move(t)) {}

    static Data make_type(Class c) {
      switch (c) {
        case Class::Null:
          return nullptr;
        case Class::Object:
          return std::map<std::string, Json>{};
        case Class::Array:
          return std::vector<Json>{};
        case Class::String:
          return std::string{};
        case Class::Floating:
          return double{};
        case Class::Integral:
          return std::int64_t{};
        case Class::Boolean:
          return bool{};
      }
      throw std::runtime_error("unknown type");
    }

    void set_type(Class c) {
      if (type() != c) {
        _d = make_type(c);
      }
    }

    Class type() const noexcept { return Class(_d.index()); }

    template <auto ClassValue, typename Visitor, typename Or>
    decltype(auto) visitor(Visitor &&v, Or &&o) const {
      if (type() == Class(ClassValue)) {
        return v(std::get<static_cast<std::size_t>(ClassValue)>(_d));
      } else {
        return o();
      }
    }

    template <auto ClassValue>
    auto &get_set_type() {
      set_type(ClassValue);
      return (std::get<static_cast<std::size_t>(ClassValue)>(_d));
    }

    auto &Map() { return get_set_type<Class::Object>(); }
    auto &Vector() { return get_set_type<Class::Array>(); }
    auto &String() { return get_set_type<Class::String>(); }
    auto &Int() { return get_set_type<Class::Integral>(); }
    auto &Float() { return get_set_type<Class::Floating>(); }
    auto &Bool() { return get_set_type<Class::Boolean>(); }

    auto Map() const noexcept {
      return std::get_if<static_cast<std::size_t>(Class::Object)>(&_d);
    }
    auto Vector() const noexcept {
      return std::get_if<static_cast<std::size_t>(Class::Array)>(&_d);
    }
    auto String() const noexcept {
      return std::get_if<static_cast<std::size_t>(Class::String)>(&_d);
    }
    auto Int() const noexcept {
      return std::get_if<static_cast<std::size_t>(Class::Integral)>(&_d);
    }
    auto Float() const noexcept {
      return std::get_if<static_cast<std::size_t>(Class::Floating)>(&_d);
    }
    auto Bool() const noexcept {
      return std::get_if<static_cast<std::size_t>(Class::Boolean)>(&_d);
    }

    Data _d;
  };

  Internal _internal;

 public:
  template <typename Container>
  class JsonWrapper {
    Container *_object = nullptr;

   public:
    JsonWrapper(Container *val) : _object(val) {}

    JsonWrapper(std::nullptr_t) {}

    typename Container::iterator begin() {
      return _object ? _object->begin() : typename Container::iterator();
    }
    typename Container::iterator end() {
      return _object ? _object->end() : typename Container::iterator();
    }
    typename Container::const_iterator begin() const {
      return _object ? _object->begin() : typename Container::iterator();
    }
    typename Container::const_iterator end() const {
      return _object ? _object->end() : typename Container::iterator();
    }
  };

  template <typename Container>
  class JsonConstWrapper {
    Container *_object = nullptr;

   public:
    JsonConstWrapper(Container *val) : _object(val) {}

    JsonConstWrapper(std::nullptr_t) {}

    typename Container::const_iterator begin() const {
      return _object ? _object->begin() : typename Container::const_iterator();
    }
    typename Container::const_iterator end() const {
      return _object ? _object->end() : typename Container::const_iterator();
    }
  };

  Json() = default;
  Json(std::nullptr_t) {}

  explicit Json(Class type) : _internal(type) {}

  Json(std::initializer_list<Json> list) : _internal(Class::Object) {
    for (auto i = list.begin(); i != list.end(); ++i, --i) {
      operator[](i->to_string()) = *std::next(i);
    }
  }

  template <typename T>
  explicit Json(T b, typename std::enable_if_t<std::is_same_v<T, bool>> * =
                         nullptr) noexcept
      : _internal(static_cast<bool>(b)) {}

  template <typename T>
  explicit Json(T i, typename std::enable_if_t<std::is_integral_v<T> &&
                                               !std::is_same_v<T, bool>> * =
                         nullptr) noexcept
      : _internal(static_cast<std::int64_t>(i)) {}

  template <typename T>
  explicit Json(T f, typename std::enable_if_t<std::is_floating_point_v<T>> * =
                         nullptr) noexcept
      : _internal(static_cast<double>(f)) {}

  template <typename T>
  explicit Json(T s,
                typename std::enable_if_t<std::is_convertible_v<T, std::string>>
                    * = nullptr)
      : _internal(static_cast<std::string>(s)) {}

  static Json Load(const std::string &);

  Json &operator[](const std::string &key) { return _internal.Map()[key]; }

  Json &operator[](const size_t index) {
    auto &vec = _internal.Vector();
    if (index >= vec.size()) {
      vec.resize(index + 1);
    }
    return vec[index];
  }

  Json &at(const std::string &key) { return operator[](key); }

  const Json &at(const std::string &key) const {
    return _internal.visitor<Class::Object>(
        [&](const auto &m) -> const Json & { return m.at(key); },
        []() -> const Json & {
          throw std::range_error("Not an object, no keys");
        });
  }

  Json &at(size_t index) { return operator[](index); }

  const Json &at(size_t index) const {
    return _internal.visitor<Class::Array>(
        [&](const auto &m) -> const Json & { return m.at(index); },
        []() -> const Json & {
          throw std::range_error("Not an array, no indexes");
        });
  }

  auto length() const noexcept {
    return _internal.visitor<Class::Array>(
        [&](const auto &m) { return static_cast<int>(m.size()); },
        []() { return -1; });
  }

  bool has_key(const std::string &key) const noexcept {
    return _internal.visitor<Class::Object>(
        [&](const auto &m) { return m.count(key) != 0; },
        []() { return false; });
  }

  int size() const noexcept {
    if (auto m = _internal.Map(); m != nullptr) {
      return static_cast<int>(m->size());
    }
    if (auto v = _internal.Vector(); v != nullptr) {
      return static_cast<int>(v->size());
    } else {
      return -1;
    }
  }

  Class JsonType() const noexcept { return _internal.type(); }

  bool is_null() const noexcept { return _internal.type() == Class::Null; }

  std::string to_string() const noexcept {
    return _internal.visitor<Class::String>([](const auto &o) { return o; },
                                            []() { return std::string{}; });
  }

  double to_float() const noexcept {
    return _internal.visitor<Class::Floating>([](const auto &o) { return o; },
                                              []() { return double{}; });
  }

  std::int64_t to_int() const noexcept {
    return _internal.visitor<Class::Integral>([](const auto &o) { return o; },
                                              []() { return std::int64_t{}; });
  }

  bool to_bool() const noexcept {
    return _internal.visitor<Class::Boolean>([](const auto &o) { return o; },
                                             []() { return false; });
  }
};

struct JsonParser {
  static bool isspace(const char c) noexcept { return ::isspace(c) != 0; }

  static void consume_ws(const std::string &str, size_t offset) {
    while (isspace(str.at(offset)) && offset <= str.size()) {
      ++offset;
    }
  }

  static Json parse_object(const std::string &str, size_t &offset) {}

  static Json parse_array(const std::string &str, size_t &offset) {}

  static Json parse_string(const std::string &str, size_t &offset) {}

  static Json parse_number(const std::string &str, size_t &offset) {}

  static Json parse_bool(const std::string &str, size_t &offset) {}

  static Json parse_null(const std::string &str, size_t &offset) {
    if (str.substr(offset, 4) != "null") {
      throw std::runtime_error(
          std::string("[Json Error] Null: Expected 'null', found '") +
          str.substr(offset, 4) + "'");
    }
    offset += 4;
    return Json();
  }

  static Json parse_next(const std::string &str, size_t &offset) {
    consume_ws(str, offset);
    char value = str.at(offset);
    switch (value) {
      case '[':
        return parse_array(str, offset);
      case '{':
        return parse_object(str, offset);
      case '\"':
        return parse_string(str, offset);
      case 't':
      case 'f':
        return parse_bool(str, offset);
      case 'n':
        return parse_null(str, offset);
      default:
        if ((value <= '9' && value >= '0') || value == '-') {
          return parse_number(str, offset);
        }
        throw std::runtime_error(
            std::string("[Json Error] Parse: Unexpected starting character '") +
            value + "'");
    }
  }
};

inline Json Json::Load(const std::string &str) {
  size_t offset = 0;
  return JsonParser::parse_next(str, offset);
}

PICIDAE_NAMESPACE_END(json)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_JSON_JSON_HPP__