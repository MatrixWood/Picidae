#ifndef KOALA_CALLABLE_TRAITS_HPP__
#define KOALA_CALLABLE_TRAITS_HPP__

#include <memory>

namespace koala
{
  namespace detail
  {
    template <typename Class, typename... Param>
    struct Constructor {
      template <typename... Inner>
      std::shared_ptr<Class> operator () (Inner&&... inner) const {
        return std::make_shared<Class>(std::forward<Inner>(inner)...);
      }
    };

    template <typename Ret, typename Class, typename... Param>
    struct ConstCaller {
      explicit ConstCaller(Ret (Class::*t_func)(Param...) const)
        : _func(t_func)
      { }

      template <typename... Inner>
      Ret operator () (const Class& o, Inner&&... inner) const {
        return (o.*_func)(std::forward<Inner>(inner)...);
      }

      Ret (Class::*_func)(Param...) const;
    };

    template <typename Ret, typename... Param>
    struct FuncCaller {
      explicit FuncCaller(Ret (*t_func)(Param...))
        : _func(t_func)
      { }

      template <typename... Inner>
      Ret operator () (Inner&&... inner) const {
        return (_func)(std::forward<Inner>(inner)...);
      }

      Ret (*_func)(Param...);
    };

    template <typename Ret, typename Class, typename... Param>
    struct Caller {
      explicit Caller(Ret (Class::*t_func)(Param...))
        : _func(t_func)
      { }

      template <typename... Inner>
      Ret operator () (Class& o, Inner&&... inner) const {
        return (o.*_func)(std::forward<Inner>(inner)...);
      }

      Ret (Class::*_func)(Param...);
    };

    template <typename T>
    struct Arity { };

    template <typename Ret, typename... Param>
    struct Arity<Ret(Param...)> {
      static const size_t arity = sizeof...(Param);
    };

    template <typename T>
    struct FunctionSignature { };

    template <typename Ret, typename... Param>
    struct FunctionSignature<Ret(Param...)> {
      using ReturnType = Ret;
      using Signature = Ret()(Param...);
    };

    template <typename Ret, typename Class, typename... Param>
    struct FunctionSignature<Ret (Class::*)(Param...)> {
      using ReturnType = Ret;
      using Signature = Ret()(Param...);
    };

    template <typename T>
    struct CallableTraits {
      using Signature = typename FunctionSignature<decltype(&T::operator())>::Signature;
      using ReturnType = typename FunctionSignature<decltype(&T::operator())>::ReturnType;
    };
  } // namespace detail
  
} // namespace koala


#endif
