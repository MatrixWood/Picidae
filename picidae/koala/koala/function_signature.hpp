#ifndef KOALA_FUNCTION_SIGNATURE_HPP__
#define KOALA_FUNCTION_SIGNATURE_HPP__

#include <type_traits>

namespace koala::detail
{
  template <typename... Param>
  struct FunctionParams
  { };

  template <typename Ret,
            typename Params,
            bool IsNoExcept = false,
            bool IsMember = false,
            bool IsMemberObject = false,
            bool IsObject = false>
  struct FunctionSignature {
    using ParamTypes = Params;
    using ReturnType = Ret;

    constexpr static const bool is_object = IsObject;
    constexpr static const bool is_member_object = IsMemberObject;
    constexpr static const bool is_noexcept = IsNoExcept;

    template <typename T>
    constexpr FunctionSignature(T&&) noexcept { }
    constexpr FunctionSignature() noexcept = default;
  };

  template <typename Ret, typename... Param>
  FunctionSignature(Ret (*f)(Param...))
    -> FunctionSignature<Ret, FunctionParams<Param...>>;

  template <typename Ret, typename... Param>
  FunctionSignature(Ret (*f)(Param...) noexcept)
    -> FunctionSignature<Ret, FunctionParams<Param...>, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile)
    -> FunctionSignature<Ret, FunctionParams<volatile Class&, Param...>, false, true>;
  
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile noexcept)
    -> FunctionSignature<Ret, FunctionParams<volatile Class&, Param...>, true, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile const)
    -> FunctionSignature<Ret, FunctionParams<volatile const Class&, Param...>, false, true>;
  
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile const noexcept)
    -> FunctionSignature<Ret, FunctionParams<volatile const Class&, Param...>, true, true>;
  
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...))
    -> FunctionSignature<Ret, FunctionParams<Class&, Param...>, false, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) noexcept)
    -> FunctionSignature<Ret, FunctionParams<Class&, Param...>, true, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) const)
    -> FunctionSignature<Ret, FunctionParams<const Class&, Param...>, false, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) const noexcept)
    -> FunctionSignature<Ret, FunctionParams<const Class&, Param...>, true, true>;

  // &
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile &)
    -> FunctionSignature<Ret, FunctionParams<volatile Class&, Param...>, false, true>;
  
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile & noexcept)
    -> FunctionSignature<Ret, FunctionParams<volatile Class&, Param...>, true, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile const &)
    -> FunctionSignature<Ret, FunctionParams<volatile const Class&, Param...>, false, true>;
  
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile const & noexcept)
    -> FunctionSignature<Ret, FunctionParams<volatile const Class&, Param...>, true, true>;
  
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) &)
    -> FunctionSignature<Ret, FunctionParams<Class&, Param...>, false, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) & noexcept)
    -> FunctionSignature<Ret, FunctionParams<Class&, Param...>, true, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) const &)
    -> FunctionSignature<Ret, FunctionParams<const Class&, Param...>, false, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) const & noexcept)
    -> FunctionSignature<Ret, FunctionParams<const Class&, Param...>, true, true>;

  // &&
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile &&)
    -> FunctionSignature<Ret, FunctionParams<volatile Class&&, Param...>, false, true>;
  
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile && noexcept)
    -> FunctionSignature<Ret, FunctionParams<volatile Class&&, Param...>, true, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile const &&)
    -> FunctionSignature<Ret, FunctionParams<volatile const Class&&, Param...>, false, true>;
  
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) volatile const && noexcept)
    -> FunctionSignature<Ret, FunctionParams<volatile const Class&&, Param...>, true, true>;
  
  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) &&)
    -> FunctionSignature<Ret, FunctionParams<Class&&, Param...>, false, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) && noexcept)
    -> FunctionSignature<Ret, FunctionParams<Class&&, Param...>, true, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) const &&)
    -> FunctionSignature<Ret, FunctionParams<const Class&&, Param...>, false, true>;

  template <typename Ret, typename Class, typename... Param>
  FunctionSignature(Ret (Class::*f)(Param...) const && noexcept)
    -> FunctionSignature<Ret, FunctionParams<const Class&&, Param...>, true, true>;

  template <typename Ret, typename Class>
  FunctionSignature(Ret Class::*f) -> FunctionSignature<Ret, FunctionParams<Class&>, true, true, true>;

  template <class, class = std::void_t<>>
  struct has_call_operator : std::false_type { };

  template <class T>
  struct has_call_operator<T, std::void_t<decltype(&T::operator())>> : std::true_type { };

  template <typename Func>
  auto function_signature(const Func& f) {
    if constexpr (has_call_operator<Func>::value) {
      return FunctionSignature<typename decltype(FunctionSignature{&std::decay_t<Func>::operator()})::ReturnType,
                               typename decltype(FunctionSignature{&std::decay_t<Func>::operator()})::ParamTypes,
                               decltype(FunctionSignature{&std::decay_t<Func>::operator()})::is_noexcept,
                               false,
                               false,
                               true>{};
    } else {
      return FunctionSignature{f};
    }
  }
} // namespace koala::detail

#endif
