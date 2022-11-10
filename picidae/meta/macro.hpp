#ifndef PICIDAE_MACRO_HPP__
#define PICIDAE_MACRO_HPP__

#include <type_traits>

namespace picidae {

// from
// https://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c
#define ENUM_FLAG_OPERATOR(T, X)                                              \
  inline T operator X(T lhs, T rhs) {                                         \
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs)         \
                              X static_cast<std::underlying_type_t<T>>(rhs)); \
  }

#define ENUM_FLAGS(T)                                                  \
  enum class T;                                                        \
  inline T operator~(T t) {                                            \
    return static_cast<T>(~static_cast<std::underlying_type_t<T>>(t)); \
  }                                                                    \
  ENUM_FLAG_OPERATOR(T, |)                                             \
  ENUM_FLAG_OPERATOR(T, ^)                                             \
  ENUM_FLAG_OPERATOR(T, &)                                             \
  enum class T

}  // namespace picidae

#endif  // PICIDAE_MACRO_HPP__
