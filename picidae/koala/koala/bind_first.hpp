#ifndef KOALA_BIND_FIRST_HPP__
#define KOALA_BIND_FIRST_HPP__

#include <functional>

namespace koala
{
  namespace detail
  {
    template <typename T>
    constexpr T* get_pointer(T* t) noexcept {
      return t;
    }

    template <typename T>
    T* get_pointer(const std::reference_wrapper<T>& t) noexcept {
      return &t.get();
    }

    template <typename O, typename Ret, typename P1, typename... Param>
    constexpr auto bind_first(Ret (*f)(P1, Param...), O&& o) {
      return [f, o = std::forward<O>(o)] (Param... param) -> Ret {
        return f(o, std::forward<Param>(param)...);
      };
    }

    template<typename O, typename Ret, typename Class, typename... Param>
    constexpr auto bind_first(Ret (Class::*f)(Param...), O &&o) {
      return [f, o = std::forward<O>(o)](Param... param)-> Ret {
        return (get_pointer(o)->*f)(std::forward<Param>(param)...);
      };
    }

    template<typename O, typename Ret, typename Class, typename... Param>
    constexpr auto bind_first(Ret (Class::*f)(Param...) const, O &&o) {
      return [f, o = std::forward<O>(o)](Param... param) -> Ret {
        return (get_pointer(o)->*f)(std::forward<Param>(param)...);
      };
    }

    template<typename O, typename Ret, typename P1, typename... Param>
    auto bind_first(const std::function<Ret(P1, Param...)> &f, O &&o) {
      return [f, o = std::forward<O>(o)](Param... param) -> Ret {
        return f(o, std::forward<Param>(param)...);
      };
    }

    template<typename F, typename O, typename Ret, typename Class, typename P1, typename... Param>
    constexpr auto bind_first(const F &fo, O &&o, Ret (Class::*f)(P1, Param...) const) {
      return [fo, o = std::forward<O>(o), f](Param... param) -> Ret {
        return (fo.*f)(o, std::forward<Param>(param)...);
      };
    }

    template<typename F, typename O>
    constexpr auto bind_first(const F &f, O &&o) {
      return bind_first(f, std::forward<O>(o), &F::operator());
    }
  } // namespace detail
  
} // namespace koala



#endif
