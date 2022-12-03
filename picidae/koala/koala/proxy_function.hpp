#ifndef KOALA_PROXY_FUNCTION_HPP__
#define KOALA_PROXY_FUNCTION_HPP__

#include <cassert>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "define.hpp"
#include "boxed_value.hpp"
#include "boxed_number.hpp"
#include "function_params.hpp"
#include "type_info.hpp"

namespace koala
{
  namespace detail
  {
    template <typename Ret, typename... Params>
    std::vector<TypeInfo> build_param_type_list(Ret (*)(Params...)) {
      return {user_type<Ret>(), user_type<Params>()...};
    }

    template <typename Callable, typename Ret, typename... Params, size_t... I>
    Ret call_func(Ret (*)(Params...),
                  std::index_sequence<I...>,
                  const Callable& f,
                  [[maybe_unused]] const koala::FunctionParams& params) {
      return f(static_cast<Params>(params[i])...);
    }

    //template <typename Callable, typename Ret, typename... Params>
    //BoxedValue call_func(Ret (*sig)(Params...)
    //                     const Callable& f,
    //                     const koala::FunctionParams& params) {
    //  //if constexpr (std::is_same_v<Ret, void>) {
    //  //}
    //}
  } // namespace detail
  
  struct TypeConversionsState;
  struct AstNode;
  using AstNodePtr = std::unique_ptr<AstNode>;
  struct ParamTypes;

  class ProxyFunctionBase {
   public:
    virtual ~ProxyFunctionBase() = default;

    BoxedValue operator () (const FunctionParams& params, const koala::TypeConversionsState& t_conversions) const {
      if (_arity < 0 || size_t(_arity) == params.size()) {
        return do_call(params, t_conversions);
      }
    }

    const std::vector<TypeInfo>& get_param_types() const noexcept { return _types; }

    virtual bool operator == (const ProxyFunctionBase&) const noexcept = 0;

    virtual bool call_match(const FunctionParams& vals, const TypeConversionsState& t_conversions) const = 0;

    virtual bool is_attribute_function() const noexcept { return false; }

    bool has_arithmetic_param() const noexcept { return _has_arithmetic_param; }

    virtual std::vector<std::shared_ptr<const ProxyFunctionBase>> get_contained_functions() const {
      return std::vector<std::shared_ptr<const ProxyFunctionBase>>();
    }

    static bool compare_type_to_param(const TypeInfo& ti, const BoxedValue& bv, 
                                      const TypeConversionsState& t_conversions) noexcept {
      const auto boxed_value_ti = user_type<BoxedValue>();
      const auto boxed_number_ti = user_type<BoxedNumber>();
      const auto function_ti = user_type<std::shared_ptr<const ProxyFunctionBase>>();

      if (ti.is_undef() || ti.bare_equal(boxed_value_ti) ||
          (!bv.get_type_info().is_undef() &&
           ((ti.bare_equal(boxed_number_ti) && bv.get_type_info().is_arithmetic()) ||
            ti.bare_equal(bv.get_type_info()) ||
            bv.get_type_info().bare_equal(function_ti) ||
            t_conversions->convert(ti, bv.get_type_info())))) {
        return true;
      } else {
        return false;
      }
    }

    virtual bool compare_first_type(const BoxedValue& bv, const TypeConversionsState& t_conversions) const noexcept {
      return compare_type_to_param(_types[1], bv, t_conversions);
    }

   protected:
    virtual BoxedValue do_call(const FunctionParams& params, const TypeConversionsState& t_conversions) const = 0;

    ProxyFunctionBase(std::vector<TypeInfo> t_types, int t_arity)
      : _types(std::move(t_types)),
        _arity(t_arity),
        _has_arithmetic_param(false)
    {
      for (size_t i = 1; i < _types.size(); ++i) {
        if (_types[i].is_arithmetic()) {
          _has_arithmetic_param = true;
          return;
        }
      }
    }

    std::vector<TypeInfo> _types;
    int _arity;
    bool _has_arithmetic_param;
  };

  using ProxyFunction = std::shared_ptr<ProxyFunctionBase>;
  using ConstProxyFunction = std::shared_ptr<const ProxyFunctionBase>;

  class DynamicProxyFunction : public ProxyFunctionBase {
   public:
    DynamicProxyFunction(const int t_arity,
                         std::shared_ptr<AstNode> t_parsenode,
                         ParamTypes t_param_types = ParamTypes(),)
  };

} // namespace koala

#endif
