#ifndef KOALA_TYPE_CONVERSIONS_HPP__
#define KOALA_TYPE_CONVERSIONS_HPP__

#include <algorithm>
#include <atomic>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "boxed_value.hpp"
#include "boxed_number.hpp"
#include "type_info.hpp"
#include "utility.hpp"

namespace koala
{
  namespace exception
  {
    class bad_boxed_cast : public std::bad_cast {
    public:
      bad_boxed_cast(TypeInfo t_from, const std::type_info &t_to, utility::StaticString t_what) noexcept
          : from(t_from)
          , to(&t_to)
          , m_what(std::move(t_what)) {
      }

      bad_boxed_cast(TypeInfo t_from, const std::type_info &t_to) noexcept
          : from(t_from)
          , to(&t_to)
          , m_what("Cannot perform boxed_cast") {
      }

      explicit bad_boxed_cast(utility::StaticString t_what) noexcept
          : m_what(std::move(t_what)) {
      }

      bad_boxed_cast(const bad_boxed_cast &) noexcept = default;
      ~bad_boxed_cast() noexcept override = default;

      /// \brief Description of what error occurred
      const char *what() const noexcept override { return m_what.c_str(); }

      TypeInfo from; ///< TypeInfo contained in the BoxedValue
      const std::type_info *to = nullptr; ///< std::type_info of the desired (but failed) result type

    private:
      utility::StaticString m_what;
    };
  } // namespace exception
  
  struct TypeConversionsState;

  namespace detail
  {
    template <typename T>
    constexpr T* throw_if_null(T* t) {
      if (t) return t;
      throw std::runtime_error("Attempted to dereference null BoxedValue");
    }

    template <typename T>
    static const T* verify_type_no_throw(const BoxedValue& ob, const std::type_info& ti,
                                         const T* ptr) {
      if (ob.get_type_info() == ti) return ptr;
    }

    template <typename T>
    static T* verify_type_no_throw(const BoxedValue& ob, const std::type_info& ti,
                                   T* ptr) {
      if (!ob.is_const() && ob.get_type_info() == ti) return ptr;
    };

    template <typename T>
    static const T* verify_type(const BoxedValue& ob, const std::type_info& ti,
                                const T* ptr) {
      if (ob.get_type_info().bare_equal_type_info(ti)) {
        return throw_if_null(ptr);
      }
    }

    template <typename T>
    static T* verify_type(const BoxedValue& ob, const std::type_info& ti,
                          T* ptr) {
      if (!ob.is_const() && ob.get_type_info().bare_equal_type_info(ti)) {
        return throw_if_null(ptr);
      }
    }

    template <typename Result>
    struct CastHelperInner {
      static Result cast(const BoxedValue& ob, const TypeConversionsState*) {
        return *static_cast<const Result*>(verify_type(ob, typeid(Result), ob.get_const_ptr()));
      }
    };

    template <typename Result>
    struct CastHelperInner<const Result> : CastHelperInner<Result>
    { };

    template<typename Result>
    struct CastHelperInner<const Result *> {
      static const Result *cast(const BoxedValue &ob, const TypeConversionsState *) {
        return static_cast<const Result *>(verify_type_no_throw(ob, typeid(Result), ob.get_const_ptr()));
      }
    };

    /// CastHelperInner for casting to a * type
    template<typename Result>
    struct CastHelperInner<Result *> {
      static Result *cast(const BoxedValue &ob, const TypeConversionsState *) {
        return static_cast<Result *>(verify_type_no_throw(ob, typeid(Result), ob.get_ptr()));
      }
    };

    template<typename Result>
    struct CastHelperInner<Result *const &> : public CastHelperInner<Result *> {
    };

    template<typename Result>
    struct CastHelperInner<const Result *const &> : public CastHelperInner<const Result *> {
    };

    /// CastHelperInner for casting to a & type
    template<typename Result>
    struct CastHelperInner<const Result &> {
      static const Result &cast(const BoxedValue &ob, const TypeConversionsState *) {
        return *static_cast<const Result *>(verify_type(ob, typeid(Result), ob.get_const_ptr()));
      }
    };

    /// CastHelperInner for casting to a & type
    template<typename Result>
    struct CastHelperInner<Result &> {
      static Result &cast(const BoxedValue &ob, const TypeConversionsState *) {
        return *static_cast<Result *>(verify_type(ob, typeid(Result), ob.get_ptr()));
      }
    };

    /// CastHelperInner for casting to a && type
    template<typename Result>
    struct CastHelperInner<Result &&> {
      static Result &&cast(const BoxedValue &ob, const TypeConversionsState *) {
        return std::move(*static_cast<Result *>(verify_type(ob, typeid(Result), ob.get_ptr())));
      }
    };

    /// CastHelperInner for casting to a std::unique_ptr<> && type
    /// \todo Fix the fact that this has to be in a shared_ptr for now
    template<typename Result>
    struct CastHelperInner<std::unique_ptr<Result> &&> {
      static std::unique_ptr<Result> &&cast(const BoxedValue &ob, const TypeConversionsState *) {
        return std::move(*(static_cast<std::shared_ptr<std::unique_ptr<Result>>>(ob.get())));
      }
    };

    /// CastHelperInner for casting to a std::unique_ptr<> & type
    /// \todo Fix the fact that this has to be in a shared_ptr for now
    template<typename Result>
    struct CastHelperInner<std::unique_ptr<Result> &> {
      static std::unique_ptr<Result> &cast(const BoxedValue &ob, const TypeConversionsState *) {
        return *(static_cast<std::shared_ptr<std::unique_ptr<Result>>>(ob.get()));
      }
    };

    /// CastHelperInner for casting to a std::unique_ptr<> & type
    /// \todo Fix the fact that this has to be in a shared_ptr for now
    template<typename Result>
    struct CastHelperInner<const std::unique_ptr<Result> &> {
      static std::unique_ptr<Result> &cast(const BoxedValue &ob, const TypeConversionsState *) {
        return *(static_cast<std::shared_ptr<std::unique_ptr<Result>>>(ob.get()));
      }
    };

    /// CastHelperInner for casting to a std::shared_ptr<> type
    template<typename Result>
    struct CastHelperInner<std::shared_ptr<Result>> {
      static auto cast(const BoxedValue &ob, const TypeConversionsState *) { return static_cast<std::shared_ptr<Result>>(ob.get()); }
    };

    /// CastHelperInner for casting to a std::shared_ptr<const> type
    template<typename Result>
    struct CastHelperInner<std::shared_ptr<const Result>> {
      static auto cast(const BoxedValue &ob, const TypeConversionsState *) {
        if (!ob.get_type_info().is_const()) {
          return std::const_pointer_cast<const Result>(static_cast<std::shared_ptr<Result>>(ob.get()));
        } else {
          return static_cast<std::shared_ptr<const Result>>(ob.get());
        }
      }
    };

    /// CastHelperInner for casting to a const std::shared_ptr<> & type
    template<typename Result>
    struct CastHelperInner<const std::shared_ptr<Result>> : CastHelperInner<std::shared_ptr<Result>> {
    };

    template<typename Result>
    struct CastHelperInner<const std::shared_ptr<Result> &> : CastHelperInner<std::shared_ptr<Result>> {
    };

    //template<typename Result>
    //struct CastHelperInner<std::shared_ptr<Result> &> {
    //  static_assert(!std::is_const<Result>::value, "Non-const reference to std::shared_ptr<const T> is not supported");
    //  static auto cast(const BoxedValue &ob, const TypeConversionsState *) {
    //    std::shared_ptr<Result> &res = static_cast<std::shared_ptr<Result>>(ob.get());
    //    return ob.pointer_sentinel(res);
    //  }
    //};

    /// CastHelperInner for casting to a const std::shared_ptr<const> & type
    template<typename Result>
    struct CastHelperInner<const std::shared_ptr<const Result>> : CastHelperInner<std::shared_ptr<const Result>> {
    };

    template<typename Result>
    struct CastHelperInner<const std::shared_ptr<const Result> &> : CastHelperInner<std::shared_ptr<const Result>> {
    };

    /// CastHelperInner for casting to a BoxedValue type
    template<>
    struct CastHelperInner<BoxedValue> {
      static BoxedValue cast(const BoxedValue &ob, const TypeConversionsState *) { return ob; }
    };

    /// CastHelperInner for casting to a BoxedValue & type
    template<>
    struct CastHelperInner<BoxedValue &> {
      static std::reference_wrapper<BoxedValue> cast(const BoxedValue &ob, const TypeConversionsState *) {
        return std::ref(const_cast<BoxedValue &>(ob));
      }
    };

    /// CastHelperInner for casting to a const BoxedValue & type
    template<>
    struct CastHelperInner<const BoxedValue> : CastHelperInner<BoxedValue> {
    };

    template<>
    struct CastHelperInner<const BoxedValue &> : CastHelperInner<BoxedValue> {
    };

    /// CastHelperInner for casting to a std::reference_wrapper type
    template<typename Result>
    struct CastHelperInner<std::reference_wrapper<Result>> : CastHelperInner<Result &> {
    };

    template<typename Result>
    struct CastHelperInner<const std::reference_wrapper<Result>> : CastHelperInner<Result &> {
    };

    template<typename Result>
    struct CastHelperInner<const std::reference_wrapper<Result> &> : CastHelperInner<Result &> {
    };

    template<typename Result>
    struct CastHelperInner<std::reference_wrapper<const Result>> : CastHelperInner<const Result &> {
    };

    template<typename Result>
    struct CastHelperInner<const std::reference_wrapper<const Result>> : CastHelperInner<const Result &> {
    };

    template<typename Result>
    struct CastHelperInner<const std::reference_wrapper<const Result> &> : CastHelperInner<const Result &> {
    };

    /// The exposed CastHelper object that by default just calls the CastHelperInner
    template<typename T>
    struct CastHelper {
      static decltype(auto) cast(const BoxedValue &ob, const TypeConversionsState *t_conversions) {
        return (CastHelperInner<T>::cast(ob, t_conversions));
      }
    };






    class TypeConversionBase {
     public:
      virtual BoxedValue convert(const BoxedValue& from) const = 0;
      virtual BoxedValue convert_down(const BoxedValue& to) const = 0;

      const TypeInfo& to() const noexcept { return _to; }
      const TypeInfo& from() const noexcept { return _from; }

      virtual bool bidir() const noexcept { return true; }

      virtual ~TypeConversionBase() = default;

     protected:
      TypeConversionBase(TypeInfo t_to, TypeInfo t_from)
        : _to(std::move(t_to)),
          _from(std::move(t_from))
      { }

     private:
      const TypeInfo _to;
      const TypeInfo _from;
    };

    template<typename From, typename To>
    class Static_Caster {
    public:
      static BoxedValue cast(const BoxedValue &t_from) {
        if (t_from.get_type_info().bare_equal(koala::user_type<From>())) {
          if (t_from.is_pointer()) {
            if (t_from.is_const()) {
              return BoxedValue([&]() {
                if (auto data
                    = std::static_pointer_cast<const To>(detail::CastHelper<std::shared_ptr<const From>>::cast(t_from, nullptr))) {
                  return data;
                } else {
                  throw std::bad_cast();
                }
              }());
            } else {
              return BoxedValue([&]() {
                if (auto data = std::static_pointer_cast<To>(detail::CastHelper<std::shared_ptr<From>>::cast(t_from, nullptr))) {
                  return data;
                } else {
                  throw std::bad_cast();
                }
              }());
            }
          } else {
            if (t_from.is_const()) {
              const From &d = detail::CastHelper<const From &>::cast(t_from, nullptr);
              const To &data = static_cast<const To &>(d);
              return BoxedValue(std::cref(data));
            } else {
              From &d = detail::CastHelper<From &>::cast(t_from, nullptr);
              To &data = static_cast<To &>(d);
              return BoxedValue(std::ref(data));
            }
          }
        }
      }
    };

    template<typename From, typename To>
    class Dynamic_Caster {
    public:
      static BoxedValue cast(const BoxedValue &t_from) {
        if (t_from.get_type_info().bare_equal(koala::user_type<From>())) {
          if (t_from.is_pointer()) {
            if (t_from.is_const()) {
              return BoxedValue([&]() {
                if (auto data
                    = std::dynamic_pointer_cast<const To>(detail::CastHelper<std::shared_ptr<const From>>::cast(t_from, nullptr))) {
                  return data;
                } else {
                  throw std::bad_cast();
                }
              }());
            } else {
              return BoxedValue([&]() {
                if (auto data = std::dynamic_pointer_cast<To>(detail::CastHelper<std::shared_ptr<From>>::cast(t_from, nullptr))) {
                  return data;
                } else {
                  throw std::bad_cast();
                }
              }());
            }
          } else {
            if (t_from.is_const()) {
              const From &d = detail::CastHelper<const From &>::cast(t_from, nullptr);
              const To &data = dynamic_cast<const To &>(d);
              return BoxedValue(std::cref(data));
            } else {
              From &d = detail::CastHelper<From &>::cast(t_from, nullptr);
              To &data = dynamic_cast<To &>(d);
              return BoxedValue(std::ref(data));
            }
          }
        }
      }
    };

    template<typename Base, typename Derived>
    class DynamicConversionImpl : public TypeConversionBase {
    public:
      DynamicConversionImpl()
          : TypeConversionBase(koala::user_type<Base>(), koala::user_type<Derived>()) {
      }

      BoxedValue convert_down(const BoxedValue &t_base) const override { return Dynamic_Caster<Base, Derived>::cast(t_base); }

      BoxedValue convert(const BoxedValue &t_derived) const override { return Static_Caster<Derived, Base>::cast(t_derived); }
    };

    template<typename Base, typename Derived>
    class StaticConversionImpl : public TypeConversionBase {
    public:
      StaticConversionImpl()
          : TypeConversionBase(koala::user_type<Base>(), koala::user_type<Derived>()) {
      }

      BoxedValue convert_down(const BoxedValue &t_base) const override {
      }

      bool bidir() const noexcept override { return false; }

      BoxedValue convert(const BoxedValue &t_derived) const override { return Static_Caster<Derived, Base>::cast(t_derived); }
    };

    template<typename Callable>
    class Type_Conversion_Impl : public TypeConversionBase {
    public:
      Type_Conversion_Impl(TypeInfo t_from, TypeInfo t_to, Callable t_func)
          : TypeConversionBase(t_to, t_from)
          , m_func(std::move(t_func)) {
      }

      BoxedValue convert_down(const BoxedValue &) const override {
      }

      BoxedValue convert(const BoxedValue &t_from) const override {
        return m_func(t_from);
      }

      bool bidir() const noexcept override { return false; }

    private:
      Callable m_func;
    };
  } // namespace detail

  class Type_Conversions {
  public:
    struct Conversion_Saves {
      bool enabled = false;
      std::vector<BoxedValue> saves;
    };

    struct Less_Than {
      bool operator()(const std::type_info *t_lhs, const std::type_info *t_rhs) const noexcept {
        return *t_lhs != *t_rhs && t_lhs->before(*t_rhs);
      }
    };

    Type_Conversions()
        : m_conversions()
        , m_convertableTypes()
        , m_num_types(0) {
    }

    Type_Conversions(const Type_Conversions &t_other) = delete;
    Type_Conversions(Type_Conversions &&) = delete;

    Type_Conversions &operator=(const Type_Conversions &) = delete;
    Type_Conversions &operator=(Type_Conversions &&) = delete;

    const std::set<const std::type_info *, Less_Than> &thread_cache() const {
      auto cache = m_thread_cache;
      if (cache.size() != m_num_types) {
        cache = m_convertableTypes;
      }

      return cache;
    }

    void add_conversion(const std::shared_ptr<detail::TypeConversionBase> &conversion) {
      if (find_bidir(conversion->to(), conversion->from()) != m_conversions.end()) {
      }
      m_conversions.insert(conversion);
      m_convertableTypes.insert({conversion->to().bare_type_info(), conversion->from().bare_type_info()});
      m_num_types = m_convertableTypes.size();
    }

    template<typename T>
    bool convertable_type() const noexcept {
      const auto type = user_type<T>().bare_type_info();
      return thread_cache().count(type) != 0;
    }

    template<typename To, typename From>
    bool converts() const noexcept {
      return converts(user_type<To>(), user_type<From>());
    }

    bool converts(const TypeInfo &to, const TypeInfo &from) const noexcept {
      const auto &types = thread_cache();
      if (types.count(to.bare_type_info()) != 0 && types.count(from.bare_type_info()) != 0) {
        return has_conversion(to, from);
      } else {
        return false;
      }
    }

    template<typename To>
    BoxedValue boxed_type_conversion(Conversion_Saves &t_saves, const BoxedValue &from) const {
      return boxed_type_conversion(user_type<To>(), t_saves, from);
    }

    template<typename From>
    BoxedValue boxed_type_down_conversion(Conversion_Saves &t_saves, const BoxedValue &to) const {
      return boxed_type_down_conversion(user_type<From>(), t_saves, to);
    }

    BoxedValue boxed_type_conversion(const TypeInfo &to, Conversion_Saves &t_saves, const BoxedValue &from) const {
      try {
        BoxedValue ret = get_conversion(to, from.get_type_info())->convert(from);
        if (t_saves.enabled) {
          t_saves.saves.push_back(ret);
        }
        return ret;
      } catch (const std::out_of_range &) {
      } catch (const std::bad_cast &) {}
    }

    BoxedValue boxed_type_down_conversion(const TypeInfo &from, Conversion_Saves &t_saves, const BoxedValue &to) const {
      try {
        BoxedValue ret = get_conversion(to.get_type_info(), from)->convert_down(to);
        if (t_saves.enabled) {
          t_saves.saves.push_back(ret);
        }
        return ret;
      } catch (const std::out_of_range &) {
      } catch (const std::bad_cast &) {
      }
    }

    static void enable_conversion_saves(Conversion_Saves &t_saves, bool t_val) { t_saves.enabled = t_val; }

    std::vector<BoxedValue> take_saves(Conversion_Saves &t_saves) {
      std::vector<BoxedValue> ret;
      std::swap(ret, t_saves.saves);
      return ret;
    }

    bool has_conversion(const TypeInfo &to, const TypeInfo &from) const {
      return find_bidir(to, from) != m_conversions.end();
    }

    std::shared_ptr<detail::TypeConversionBase> get_conversion(const TypeInfo &to, const TypeInfo &from) const {

      const auto itr = find(to, from);

      if (itr != m_conversions.end()) {
        return *itr;
      } else {
        throw std::out_of_range(std::string("No such conversion exists from ") + from.bare_name() + " to " + to.bare_name());
      }
    }

  private:
    std::set<std::shared_ptr<detail::TypeConversionBase>>::const_iterator find_bidir(const TypeInfo &to, const TypeInfo &from) const {
      return std::find_if(m_conversions.begin(),
                          m_conversions.end(),
                          [&to, &from](const std::shared_ptr<detail::TypeConversionBase> &conversion) -> bool {
                            return (conversion->to().bare_equal(to) && conversion->from().bare_equal(from))
                                || (conversion->bidir() && conversion->from().bare_equal(to) && conversion->to().bare_equal(from));
                          });
    }

    std::set<std::shared_ptr<detail::TypeConversionBase>>::const_iterator find(const TypeInfo &to, const TypeInfo &from) const {
      return std::find_if(m_conversions.begin(),
                          m_conversions.end(),
                          [&to, &from](const std::shared_ptr<detail::TypeConversionBase> &conversion) {
                            return conversion->to().bare_equal(to) && conversion->from().bare_equal(from);
                          });
    }

    std::set<std::shared_ptr<detail::TypeConversionBase>> get_conversions() const {

      return m_conversions;
    }

    std::set<std::shared_ptr<detail::TypeConversionBase>> m_conversions;
    std::set<const std::type_info *, Less_Than> m_convertableTypes;
    std::atomic_size_t m_num_types;
    std::set<const std::type_info *, Less_Than> m_thread_cache;
  };

  class TypeConversionsState {
  public:
    TypeConversionsState(const Type_Conversions &t_conversions, Type_Conversions::Conversion_Saves &t_saves)
        : m_conversions(t_conversions)
        , m_saves(t_saves) {
    }

    const Type_Conversions *operator->() const noexcept { return &m_conversions.get(); }

    const Type_Conversions *get() const noexcept { return &m_conversions.get(); }

    Type_Conversions::Conversion_Saves &saves() const noexcept { return m_saves; }

  private:
    std::reference_wrapper<const Type_Conversions> m_conversions;
    std::reference_wrapper<Type_Conversions::Conversion_Saves> m_saves;
  };
  
  template<typename Type>
  decltype(auto) boxed_cast(const BoxedValue &bv, const TypeConversionsState *t_conversions = nullptr) {
    if (!t_conversions || bv.get_type_info().bare_equal(user_type<Type>()) || (t_conversions && !(*t_conversions)->convertable_type<Type>())) {
      return detail::CastHelper<Type>::cast(bv, t_conversions);
    }

    if (t_conversions && (*t_conversions)->convertable_type<Type>()) {
      try {
        return (detail::CastHelper<Type>::cast((*t_conversions)->boxed_type_conversion<Type>(t_conversions->saves(), bv), t_conversions));
      } catch (...) {
        return (detail::CastHelper<Type>::cast((*t_conversions)->boxed_type_down_conversion<Type>(t_conversions->saves(), bv),
                                                t_conversions));
      }
    } else {
      throw exception::bad_boxed_cast(bv.get_type_info(), typeid(Type));
    }
    //return detail::CastHelper<Type>::cast(BoxedValue(), nullptr);
  }
  
} // namespace koala



#endif
