#ifndef KOALA_BOXED_VALUE_HPP__
#define KOALA_BOXED_VALUE_HPP__

#include <map>
#include <any>
#include <memory>
#include <type_traits>

#include "define.hpp"
#include "type_info.hpp"
#include "no_destructor.hpp"

namespace koala
{
  class BoxedValue {
   public:
    struct VoidType { };

   private:
    struct Data {
      Data(const TypeInfo& ti, std::any to, bool is_ref, const void* t_void_ptr, bool t_return_value)
        : _type_info(ti),
          _obj(std::move(to)),
          _data_ptr(ti.is_const() ? nullptr : const_cast<void*>(t_void_ptr)),
          _const_data_ptr(t_void_ptr),
          _is_ref(is_ref),
          _return_value(t_return_value)
      { }

      Data& operator = (const Data& rhs) {
        _type_info = rhs._type_info;
        _obj = rhs._obj;
        _is_ref = rhs._is_ref;
        _data_ptr = rhs._data_ptr;
        _const_data_ptr = rhs._const_data_ptr;
        _return_value = rhs._return_value;

        if (rhs._attrs) {
          _attrs = std::make_unique<std::map<std::string, std::shared_ptr<Data>>>(*rhs._attrs);
        }

        return *this;
      }

      Data(const Data&) = delete;

      Data(Data&&) = default;
      Data& operator = (Data&& rhs) = default;

      TypeInfo _type_info;
      std::any _obj;
      void* _data_ptr;
      const void* _const_data_ptr;
      std::unique_ptr<std::map<std::string, std::shared_ptr<Data>>> _attrs;
      bool _is_ref;
      bool _return_value;
    };

    struct ObjectData {
      static auto get(BoxedValue::VoidType, bool t_return_value) {
        return std::make_shared<Data>(detail::GetTypeInfo<void>::get(), std::any(), false, nullptr, t_return_value);
      }

      template <typename T>
      static auto get(const std::shared_ptr<T>* obj, bool t_return_value) {
        // TODO
      }

      template <typename T>
      static auto get(T t, bool t_return_value) {
        auto p = std::make_shared<T>(std::move(t));
        return std::make_shared<Data>(detail::GetTypeInfo<T>::get(), std::any(std::move(p)), false, p.get(), t_return_value);
      }

      static std::shared_ptr<Data> get() {
        return std::make_shared<Data>(TypeInfo(), std::any(), false, nullptr, false);
      }
    };

   public:
    template <typename T, typename = std::enable_if_t<!std::is_same_v<BoxedValue, std::decay_t<T>>>>
    explicit BoxedValue(T&& t, bool t_return_value = true)
      : _data(ObjectData::get(std::forward<T>(t), t_return_value))
    { }

    BoxedValue() = default;

    BoxedValue(BoxedValue&&) = default;
    BoxedValue& operator = (BoxedValue&&) = default;
    BoxedValue(const BoxedValue&) = default;
    BoxedValue& operator = (const BoxedValue&) = default;

    void swap(BoxedValue& rhs) noexcept { std::swap(_data, rhs._data); }

    BoxedValue assign(const BoxedValue& rhs) noexcept {
      *_data = *rhs._data;
      return *this;
    }

    const TypeInfo& get_type_info() const noexcept { return _data->_type_info; }

    bool is_undef() const noexcept { return _data->_type_info.is_undef(); }
    bool is_const() const noexcept { return _data->_type_info.is_const(); }
    bool is_type(const TypeInfo& ti) const noexcept { return _data->_type_info.bare_equal(ti); }
    bool is_null() const noexcept { return (_data->_data_ptr == nullptr && _data->_const_data_ptr == nullptr); }
    const std::any& get() const noexcept { return _data->_obj; }
    bool is_ref() const noexcept { return _data->_is_ref; }
    bool is_return_value() const noexcept { return _data->_return_value; }
    void reset_return_value() const noexcept { _data->_return_value = false; }
    bool is_pointer() const noexcept { return !is_ref(); }
    void *get_ptr() const noexcept { return _data->_data_ptr; }
    const void *get_const_ptr() const noexcept { return _data->_const_data_ptr; }

   private:
    std::shared_ptr<Data> _data = ObjectData::get();
  };

  template <typename T>
  BoxedValue var(T&& t) {
    return BoxedValue(std::forward<T>(t));
  }

  inline BoxedValue void_var() {
    static NoDestructor<BoxedValue::VoidType> singleton;
    BoxedValue::VoidType* bv = singleton.get();
    static const auto v = BoxedValue(*bv);
    return v;
  }


  namespace detail
  {
    template<typename T>
    BoxedValue const_var_impl(const T &t) {
      return BoxedValue(std::make_shared<typename std::add_const<T>::type>(t));
    }

    template<typename T>
    BoxedValue const_var_impl(T *t) {
      return BoxedValue(const_cast<typename std::add_const<T>::type *>(t));
    }

    template<typename T>
    BoxedValue const_var_impl(const std::shared_ptr<T> &t) {
      return BoxedValue(std::const_pointer_cast<typename std::add_const<T>::type>(t));
    }

    template<typename T>
    BoxedValue const_var_impl(const std::reference_wrapper<T> &t) {
      return BoxedValue(std::cref(t.get()));
    }
  } // namespace detail
  
  template<typename T>
  BoxedValue const_var(const T &t) {
    return detail::const_var_impl(t);
  }

  inline BoxedValue const_var(bool b) {
    static const auto t = detail::const_var_impl(true);
    static const auto f = detail::const_var_impl(false);

    if (b) {
      return t;
    } else {
      return f;
    }
  }
} // namespace koala

#endif
