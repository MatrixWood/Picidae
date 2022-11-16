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

  Json &operator[](const std::string &key) { return _internal.Map()[key]; }

  std::string to_string() const noexcept {
    return _internal.visitor<Class::String>([](const auto &o) { return o; },
                                            []() { return std::string{}; });
  }
};

PICIDAE_NAMESPACE_END(json)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_JSON_JSON_HPP__