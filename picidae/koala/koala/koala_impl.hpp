#ifndef KOALA_KOALA_IMPL_HPP__
#define KOALA_KOALA_IMPL_HPP__

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeinfo>
#include <utility>
#include <vector>

#include "type_info.hpp"
#include "boxed_value.hpp"

namespace koala
{
  namespace exception
  {
    class reserved_word_error : public std::runtime_error {
     public:
      explicit reserved_word_error(const std::string &t_word) noexcept
        : std::runtime_error("Reserved word not allowed in object name: " + t_word),
          m_word(t_word)
      { }

      reserved_word_error(const reserved_word_error &) = default;

      ~reserved_word_error() noexcept override = default;

      std::string word() const { return m_word; }

     private:
      std::string m_word;
    };

    /// Exception thrown in the case that an object name is invalid because it contains illegal characters
    class illegal_name_error : public std::runtime_error {
     public:
      explicit illegal_name_error(const std::string &t_name) noexcept
        : std::runtime_error("Reserved name not allowed in object name: " + t_name),
          m_name(t_name)
      { }

      illegal_name_error(const illegal_name_error &) = default;

      ~illegal_name_error() noexcept override = default;

      std::string name() const { return m_name; }

    private:
      std::string m_name;
    };

    /// Exception thrown in the case that an object name is invalid because it already exists in current context
    class name_conflict_error : public std::runtime_error {
     public:
      explicit name_conflict_error(const std::string &t_name) noexcept
        : std::runtime_error("Name already exists in current context " + t_name),
          m_name(t_name)
      { }

      name_conflict_error(const name_conflict_error &) = default;

      ~name_conflict_error() noexcept override = default;

      std::string name() const { return m_name; }

     private:
      std::string m_name;
    };
    class global_non_const : public std::runtime_error {
     public:
      global_non_const() noexcept
        : std::runtime_error("a global object must be const")
      { }

      global_non_const(const global_non_const&) = default;
      ~global_non_const() noexcept override = default;
    };
  } // namespace exception
  
  class Module {
   public:
    Module& add(TypeInfo ti, std::string name) {
      _typeinfos.emplace_back(ti, std::move(name));
      return *this;
    }

    Module& add_global_const(BoxedValue t_bv, std::string t_name) {
      if (!t_bv.is_const()) {
        throw koala::exception::global_non_const();
      }

      _globals.emplace_back(std::move(t_bv), std::move(t_name));
      return *this;
    }

    Module& eval(std::string str) {
      _evals.push_back(std::move(str));
      return *this;
    }



   private:
    std::vector<std::pair<TypeInfo, std::string>> _typeinfos;
    std::vector<std::pair<BoxedValue, std::string>> _globals;
    std::vector<std::string> _evals;
  };

  using ModulePtr = std::shared_ptr<Module>;
} // namespace koala

#endif
