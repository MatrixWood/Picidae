#ifndef PICIDAE_META_MACRO_HPP__
#define PICIDAE_META_MACRO_HPP__



#define PICIDAE_VERSION_MAJOR 0
#define PICIDAE_VERSION_MINOR 1

#define PICIDAE_NAMESPACE_BEGIN(name) namespace name {
#define PICIDAE_NAMESPACE_END(name) }

#ifndef PICIDAE_NAMESPACE
  #define PICIDAE_NAMESPACE picidae
#endif

#if !(__cplusplus == 199711L)
  #if __cplusplus >= 201402L
    #define PICIDAE_CPP14
    #if __cplusplus >= 201703L
      #define PICIDAE_CPP17
      #if __cplusplus >= 202002L
        #define PICIDAE_CPP20
      #endif
    #endif
  #endif
#endif

#ifndef PICIDAE_EXPORT
  #define PICIDAE_EXPORT
#endif

#ifndef PICIDAE_EXPORT_EXCEPTION
  #define PICIDAE_EXPORT_EXCEPTION
#endif

#ifdef PICIDAE_CPP14
  #define PICIDAE_DEPRECATED(reason) [[deprecated(reason)]]
#else
  #define PICIDAE_DEPRECATED(reasom) __attribute((deprecated(reasom)))
#endif

#ifdef PICIDAE_CPP17
  #define PICIDAE_MAYBE_UNUSED [[maybe_unused]]
#else
  #define PICIDAE_MAYBE_UNUSED __attribute((__unused__))
#endif

#include <type_traits>

// from
// https://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c
#define PICIDAE_ENUM_FLAG_OPERATOR(T, X)                                      \
  inline T operator X(T lhs, T rhs) {                                         \
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs)         \
                              X static_cast<std::underlying_type_t<T>>(rhs)); \
  }

#define PICIDAE_ENUM_FLAGS(T)                                          \
  enum class T;                                                        \
  inline T operator~(T t) {                                            \
    return static_cast<T>(~static_cast<std::underlying_type_t<T>>(t)); \
  }                                                                    \
  PICIDAE_ENUM_FLAG_OPERATOR(T, |)                                     \
  PICIDAE_ENUM_FLAG_OPERATOR(T, ^)                                     \
  PICIDAE_ENUM_FLAG_OPERATOR(T, &)                                     \
  enum class T

#define PICIDAE_STRINGIFY(x) #x
#define PICIDAE_TOSTRING(x) PICIDAE_STRINGIFY(x)
#define PICIDAE_CONCAT(first, second) first##second

#endif  // PICIDAE_META_MACRO_HPP__
