#ifndef PICIDAE_DEBUG_HPP__
#define PICIDAE_DEBUG_HPP__

#include <type_traits>
namespace picidae {

constexpr bool enable_assert = true;

template <typename...>
struct T0 {};

template <typename...>
struct T1 {};

template <typename...>
struct T2 {};

template <typename...>
struct T3 {};

template <typename...>
struct T4 {};

template <template <auto...> typename...>
struct TT0 {};

template <template <typename...> typename...>
struct TT1 {};

template <template <typename, auto...> typename...>
struct TT2 {};

template <typename T, typename U>
struct comp {
  // constexpr static auto value =
};

template <bool B>
void execute() {
  static_assert(B);
}

template <auto p, auto q>
void execute() {
  execute<p == q>();
}

template <typename T, typename U>
void execute() {
  execute<std::is_same_v<T, U>>();
}

struct EmptyType;

template <bool>
struct ComileTimeCheck;
template <>
struct ComileTimeCheck<true> {};

template <class T1, class T2>
struct StaticCheckEQ;
template <class T>
struct StaticCheckEQ<T, T> {};

template <class T1, class T2>
struct IsEQ {
  constexpr static const bool value = false;
};
template <class T>
struct IsEQ<T, T> {
  constexpr static const bool value = true;
};

template <class T1, class T2>
struct NotEQ {
  constexpr static const bool value = true;
};
template <class T>
struct NotEQ<T, T> {
  constexpr static const bool value = false;
};

}  // namespace picidae

#endif  // PICIDAE_DEBUG_HPP__