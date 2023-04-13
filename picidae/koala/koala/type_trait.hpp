#ifndef KOALA_TYPE_TRAIT_HPP__
#define KOALA_TYPE_TRAIT_HPP__

#include <type_traits>
#include <utility>

namespace koala
{
  template <typename T>
  class is_integral_or_enum {
    using UnderlyingT = std::remove_reference_t<T>;

   public:
    static const bool value =
      !std::is_class<UnderlyingT>::value &&
      !std::is_pointer<UnderlyingT>::value &&
      !std::is_floating_point<UnderlyingT>::value &&
      (std::is_enum<UnderlyingT>::value ||
       std::is_convertible<UnderlyingT, unsigned long long>::value);
  };

  /// If T is a pointer, just return it. If it is not, return T&.
  template <typename T, typename Enable = void>
  struct add_lvalue_reference_if_not_pointer {
    using type = T&;
  };
  template <typename T>
  struct add_lvalue_reference_if_not_pointer<T, std::enable_if_t<std::is_pointer<T>::value>> {
    using type = T;
  };

  template <typename T, typename Enable = void>
  struct add_const_past_pointer { using type = const T; };
  template <typename T>
  struct add_const_past_pointer<T, std::enable_if_t<std::is_pointer<T>::value>> {
    using type = const std::remove_pointer_t<T> *;
  };

  template <typename T, typename Enable = void>
  struct const_pointer_or_const_ref {
    using type = const T &;
  };
  template <typename T>
  struct const_pointer_or_const_ref<T, std::enable_if_t<std::is_pointer<T>::value>> {
    using type = typename add_const_past_pointer<T>::type;
  };

  namespace detail
  {
    template<typename T> union copy_construction_triviality_helper {
      T t;
      copy_construction_triviality_helper() = default;
      copy_construction_triviality_helper(const copy_construction_triviality_helper&) = default;
      ~copy_construction_triviality_helper() = default;
    };
    /// Internal utility to detect trivial move construction.
    template<typename T> union move_construction_triviality_helper {
      T t;
      move_construction_triviality_helper() = default;
      move_construction_triviality_helper(move_construction_triviality_helper&&) = default;
      ~move_construction_triviality_helper() = default;
    };

    template<class T>
    union trivial_helper {
      T t;
    };
  } // namespace detail
  
  /// An implementation of `std::is_trivially_copy_constructible` since we have
  /// users with STLs that don't yet include it.
  template <typename T>
  struct is_trivially_copy_constructible
      : std::is_copy_constructible<
            ::koala::detail::copy_construction_triviality_helper<T>> {};
  template <typename T>
  struct is_trivially_copy_constructible<T &> : std::true_type {};
  template <typename T>
  struct is_trivially_copy_constructible<T &&> : std::false_type {};

  /// An implementation of `std::is_trivially_move_constructible` since we have
  /// users with STLs that don't yet include it.
  template <typename T>
  struct is_trivially_move_constructible
      : std::is_move_constructible<
            ::koala::detail::move_construction_triviality_helper<T>> {};
  template <typename T>
  struct is_trivially_move_constructible<T &> : std::true_type {};
  template <typename T>
  struct is_trivially_move_constructible<T &&> : std::true_type {};


  template <typename T>
  struct is_copy_assignable {
    template<class F>
      static auto get(F*) -> decltype(std::declval<F &>() = std::declval<const F &>(), std::true_type{});
      static std::false_type get(...);
      static constexpr bool value = decltype(get((T*)nullptr))::value;
  };

  template <typename T>
  struct is_move_assignable {
    template<class F>
      static auto get(F*) -> decltype(std::declval<F &>() = std::declval<F &&>(), std::true_type{});
      static std::false_type get(...);
      static constexpr bool value = decltype(get((T*)nullptr))::value;
  };
} // namespace koala

#endif
