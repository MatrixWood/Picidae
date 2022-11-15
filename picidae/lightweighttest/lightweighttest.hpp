#ifndef PICIDAE_LIGHTWEIGHTTEST_LIGHTWEIGHTTEST_HPP__
#define PICIDAE_LIGHTWEIGHTTEST_LIGHTWEIGHTTEST_HPP__

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(lightweighttest)

#define PICIDAE_LIGHTWEIGHTTEST_NAME "[PICIDAE LWT TEST INFO] "
#define PICIDAE_LIGHTWEIGHTTEST_TERM_RED "\033[91m"
#define PICIDAE_LIGHTWEIGHTTEST_TERM_GREEN "\033[92m"

class test_result {
 public:
  test_result() : report_(false), errors_(0) {}

  ~test_result() {
    if (!report_) {
      std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED
                << PICIDAE_LIGHTWEIGHTTEST_NAME
                << "main() should return report_errors()"
                << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
      std::abort();
    }
  }

  int& errors() { return errors_; }

  void done() { report_ = true; }

 private:
  bool report_;
  int errors_;
};

inline test_result& test_results() {
  static test_result instance;
  return instance;
}

inline int& test_errors() { return test_results().errors(); }

inline bool test_impl(char const* expr, char const* file, int line,
                      char const* function, bool v) {
  if (v) {
    test_results();
    return true;
  } else {
    std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED
              << PICIDAE_LIGHTWEIGHTTEST_NAME << file << "(" << line
              << "): test '" << expr << "' failed in function '" << function
              << "'" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
    ++test_results().errors();
    return false;
  }
}

inline void error_impl(char const* msg, char const* file, int line,
                       char const* function) {
  std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
            << file << "(" << line << "): " << msg << " in function '"
            << function << "'" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
  ++test_results().errors();
}

inline void throw_failed_impl(const char* expr, char const* excep,
                              char const* file, int line,
                              char const* function) {
  std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
            << file << "(" << line << "): expression '" << expr
            << "' did not throw exception '" << excep << "' in function '"
            << function << "'" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
  ++test_results().errors();
}

inline void no_throw_failed_impl(const char* expr, const char* file, int line,
                                 const char* function) {
  std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
            << file << "(" << line << "): expression '" << expr
            << "' threw an exception in function '" << function << "'"
            << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
  ++test_results().errors();
}

inline void no_throw_failed_impl(const char* expr, const char* what,
                                 const char* file, int line,
                                 const char* function) {
  std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
            << file << "(" << line << "): expression '" << expr
            << "' threw an exception in function '" << function << "': " << what
            << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
  ++test_results().errors();
}

// specialize test output for char pointers to avoid printing as cstring
template <class T>
inline const T& test_output_impl(const T& v) {
  return v;
}
inline const void* test_output_impl(const char* v) { return v; }
inline const void* test_output_impl(const unsigned char* v) { return v; }
inline const void* test_output_impl(const signed char* v) { return v; }
inline const void* test_output_impl(char* v) { return v; }
inline const void* test_output_impl(unsigned char* v) { return v; }
inline const void* test_output_impl(signed char* v) { return v; }
template <class T>
inline const void* test_output_impl(T volatile* v) {
  return const_cast<T*>(v);
}
inline const void* test_output_impl(std::nullptr_t) { return nullptr; }

// predicates

struct lw_test_eq {
  template <typename T, typename U>
  bool operator()(const T& t, const U& u) const {
    return t == u;
  }
};

struct lw_test_ne {
  template <typename T, typename U>
  bool operator()(const T& t, const U& u) const {
    return t != u;
  }
};

struct lw_test_lt {
  template <typename T, typename U>
  bool operator()(const T& t, const U& u) const {
    return t < u;
  }
};

struct lw_test_le {
  template <typename T, typename U>
  bool operator()(const T& t, const U& u) const {
    return t <= u;
  }
};

struct lw_test_gt {
  template <typename T, typename U>
  bool operator()(const T& t, const U& u) const {
    return t > u;
  }
};

struct lw_test_ge {
  template <typename T, typename U>
  bool operator()(const T& t, const U& u) const {
    return t >= u;
  }
};

// lwt_predicate_name

template <class T>
char const* lwt_predicate_name(T const&) {
  return "~=";
}

inline char const* lwt_predicate_name(lw_test_eq const&) { return "=="; }

inline char const* lwt_predicate_name(lw_test_ne const&) { return "!="; }

inline char const* lwt_predicate_name(lw_test_lt const&) { return "<"; }

inline char const* lwt_predicate_name(lw_test_le const&) { return "<="; }

inline char const* lwt_predicate_name(lw_test_gt const&) { return ">"; }

inline char const* lwt_predicate_name(lw_test_ge const&) { return ">="; }

template <class BinaryPredicate, class T, class U>
inline bool test_with_impl(BinaryPredicate pred, char const* expr1,
                           char const* expr2, char const* file, int line,
                           char const* function, T const& t, U const& u) {
  if (pred(t, u)) {
    test_results();
    return true;
  } else {
    std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED
              << PICIDAE_LIGHTWEIGHTTEST_NAME << file << "(" << line
              << "): test '" << expr1 << " " << lwt_predicate_name(pred) << " "
              << expr2 << "' ('" << test_output_impl(t) << "' "
              << lwt_predicate_name(pred) << " '" << test_output_impl(u)
              << "') failed in function '" << function << "'"
              << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
    ++test_results().errors();
    return false;
  }
}

inline bool test_cstr_eq_impl(char const* expr1, char const* expr2,
                              char const* file, int line, char const* function,
                              char const* const t, char const* const u) {
  if (std::strcmp(t, u) == 0) {
    test_results();
    return true;
  } else {
    std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED
              << PICIDAE_LIGHTWEIGHTTEST_NAME << file << "(" << line
              << "): test '" << expr1 << " == " << expr2 << "' ('" << t
              << "' == '" << u << "') failed in function '" << function << "'"
              << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
    ++test_results().errors();
    return false;
  }
}

inline bool test_cstr_ne_impl(char const* expr1, char const* expr2,
                              char const* file, int line, char const* function,
                              char const* const t, char const* const u) {
  if (std::strcmp(t, u) != 0) {
    test_results();
    return true;
  } else {
    std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED
              << PICIDAE_LIGHTWEIGHTTEST_NAME << file << "(" << line
              << "): test '" << expr1 << " != " << expr2 << "' ('" << t
              << "' != '" << u << "') failed in function '" << function << "'"
              << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
    ++test_results().errors();
    return false;
  }
}

template <class FormattedOutputFunction, class InputIterator1,
          class InputIterator2>
bool test_all_eq_impl(FormattedOutputFunction& output, char const* file,
                      int line, char const* function,
                      InputIterator1 first_begin, InputIterator1 first_end,
                      InputIterator2 second_begin, InputIterator2 second_end) {
  InputIterator1 first_it = first_begin;
  InputIterator2 second_it = second_begin;
  typename std::iterator_traits<InputIterator1>::difference_type first_index =
      0;
  typename std::iterator_traits<InputIterator2>::difference_type second_index =
      0;
  std::size_t error_count = 0;
  const std::size_t max_count = 8;
  do {
    while ((first_it != first_end) && (second_it != second_end) &&
           (*first_it == *second_it)) {
      ++first_it;
      ++second_it;
      ++first_index;
      ++second_index;
    }
    if ((first_it == first_end) || (second_it == second_end)) {
      break;  // do-while
    }
    if (error_count == 0) {
      output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
             << file << "(" << line
             << "): Container contents differ in function '" << function
             << "':" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
    } else if (error_count >= max_count) {
      output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
             << " ..." << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
      break;
    }
    output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
           << " [" << first_index << "] '" << test_output_impl(*first_it)
           << "' != '" << test_output_impl(*second_it) << "'"
           << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
    ++first_it;
    ++second_it;
    ++first_index;
    ++second_index;
    ++error_count;
  } while (first_it != first_end);

  first_index += std::distance(first_it, first_end);
  second_index += std::distance(second_it, second_end);
  if (first_index != second_index) {
    if (error_count == 0) {
      output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
             << file << "(" << line << "): Container sizes differ in function '"
             << function << "': size(" << first_index << ") != size("
             << second_index << ")" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
    } else {
      output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
             << " [*] size(" << first_index << ") != size(" << second_index
             << ")" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
    }
    ++error_count;
  }

  if (error_count == 0) {
    test_results();
    return true;
  } else {
    output << std::endl;
    ++test_results().errors();
    return false;
  }
}

template <class FormattedOutputFunction, class InputIterator1,
          class InputIterator2, typename BinaryPredicate>
bool test_all_with_impl(FormattedOutputFunction& output, char const* file,
                        int line, char const* function,
                        InputIterator1 first_begin, InputIterator1 first_end,
                        InputIterator2 second_begin, InputIterator2 second_end,
                        BinaryPredicate predicate) {
  InputIterator1 first_it = first_begin;
  InputIterator2 second_it = second_begin;
  typename std::iterator_traits<InputIterator1>::difference_type first_index =
      0;
  typename std::iterator_traits<InputIterator2>::difference_type second_index =
      0;
  std::size_t error_count = 0;
  const std::size_t max_count = 8;
  do {
    while ((first_it != first_end) && (second_it != second_end) &&
           predicate(*first_it, *second_it)) {
      ++first_it;
      ++second_it;
      ++first_index;
      ++second_index;
    }
    if ((first_it == first_end) || (second_it == second_end)) {
      break;  // do-while
    }
    if (error_count == 0) {
      output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
             << file << "(" << line
             << "): Container contents differ in function '" << function
             << "':" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
    } else if (error_count >= max_count) {
      output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
             << " ..." << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
      break;
    }
    output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
           << " [" << first_index << "]" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
    ++first_it;
    ++second_it;
    ++first_index;
    ++second_index;
    ++error_count;
  } while (first_it != first_end);

  first_index += std::distance(first_it, first_end);
  second_index += std::distance(second_it, second_end);
  if (first_index != second_index) {
    if (error_count == 0) {
      output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
             << file << "(" << line << "): Container sizes differ in function '"
             << function << "': size(" << first_index << ") != size("
             << second_index << ")" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
    } else {
      output << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << PICIDAE_LIGHTWEIGHTTEST_NAME
             << " [*] size(" << first_index << ") != size(" << second_index
             << ")" << PICIDAE_LIGHTWEIGHTTEST_TERM_RED;
    }
    ++error_count;
  }

  if (error_count == 0) {
    test_results();
    return true;
  } else {
    output << std::endl;
    ++test_results().errors();
    return false;
  }
}

inline int report_errors() {
  test_result& result = test_results();
  result.done();

  int errors = result.errors();

  if (errors == 0) {
    std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_GREEN
              << PICIDAE_LIGHTWEIGHTTEST_NAME << "No errors detected."
              << PICIDAE_LIGHTWEIGHTTEST_TERM_GREEN << std::endl;
  } else {
    std::cerr << PICIDAE_LIGHTWEIGHTTEST_TERM_RED
              << PICIDAE_LIGHTWEIGHTTEST_NAME << errors << " error"
              << (errors == 1 ? "" : "s") << " detected."
              << PICIDAE_LIGHTWEIGHTTEST_TERM_RED << std::endl;
  }

  // `return report_errors();` from main only supports 8 bit exit codes
  return errors < 256 ? errors : 255;
}

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__) ||               \
    defined(__clang__)
#define PICIDAE_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define PICIDAE_CURRENT_FUNCTION __func__
#else
#define PICIDAE_CURRENT_FUNCTION "(unknown)"
#endif

#define PICIDAE_TEST(expr)                                          \
  (::picidae::lightweighttest::test_impl(#expr, __FILE__, __LINE__, \
                                         PICIDAE_CURRENT_FUNCTION,  \
                                         (expr) ? true : false))
#define PICIDAE_TEST_NOT(expr) PICIDAE_TEST(!(expr))

#define PICIDAE_ERROR(msg)                                         \
  (::picidae::lightweighttest::error_impl(msg, __FILE__, __LINE__, \
                                          PICIDAE_CURRENT_FUNCTION))

#define PICIDAE_TEST_WITH(expr1, expr2, predicate)                             \
  (::picidae::lightweighttest::test_with_impl(                                 \
      predicate, #expr1, #expr2, __FILE__, __LINE__, PICIDAE_CURRENT_FUNCTION, \
      expr1, expr2))

#define PICIDAE_TEST_EQ(expr1, expr2)                                     \
  (::picidae::lightweighttest::test_with_impl(                            \
      ::picidae::lightweighttest::lw_test_eq(), #expr1, #expr2, __FILE__, \
      __LINE__, PICIDAE_CURRENT_FUNCTION, expr1, expr2))
#define PICIDAE_TEST_NE(expr1, expr2)                                     \
  (::picidae::lightweighttest::test_with_impl(                            \
      ::picidae::lightweighttest::lw_test_ne(), #expr1, #expr2, __FILE__, \
      __LINE__, PICIDAE_CURRENT_FUNCTION, expr1, expr2))

#define PICIDAE_TEST_LT(expr1, expr2)                                     \
  (::picidae::lightweighttest::test_with_impl(                            \
      ::picidae::lightweighttest::lw_test_lt(), #expr1, #expr2, __FILE__, \
      __LINE__, PICIDAE_CURRENT_FUNCTION, expr1, expr2))
#define PICIDAE_TEST_LE(expr1, expr2)                                     \
  (::picidae::lightweighttest::test_with_impl(                            \
      ::picidae::lightweighttest::lw_test_le(), #expr1, #expr2, __FILE__, \
      __LINE__, PICIDAE_CURRENT_FUNCTION, expr1, expr2))
#define PICIDAE_TEST_GT(expr1, expr2)                                     \
  (::picidae::lightweighttest::test_with_impl(                            \
      ::picidae::lightweighttest::lw_test_gt(), #expr1, #expr2, __FILE__, \
      __LINE__, PICIDAE_CURRENT_FUNCTION, expr1, expr2))
#define PICIDAE_TEST_GE(expr1, expr2)                                     \
  (::picidae::lightweighttest::test_with_impl(                            \
      ::picidae::lightweighttest::lw_test_ge(), #expr1, #expr2, __FILE__, \
      __LINE__, PICIDAE_CURRENT_FUNCTION, expr1, expr2))

#define PICIDAE_TEST_CSTR_EQ(expr1, expr2)                                 \
  (::picidae::lightweighttest::test_cstr_eq_impl(                          \
      #expr1, #expr2, __FILE__, __LINE__, PICIDAE_CURRENT_FUNCTION, expr1, \
      expr2))
#define PICIDAE_TEST_CSTR_NE(expr1, expr2)                                 \
  (::picidae::lightweighttest::test_cstr_ne_impl(                          \
      #expr1, #expr2, __FILE__, __LINE__, PICIDAE_CURRENT_FUNCTION, expr1, \
      expr2))

#define PICIDAE_TEST_ALL_EQ(begin1, end1, begin2, end2)                        \
  (::picidae::lightweighttest::test_all_eq_impl(std::cerr, __FILE__, __LINE__, \
                                                PICIDAE_CURRENT_FUNCTION,      \
                                                begin1, end1, begin2, end2))
#define PICIDAE_TEST_ALL_WITH(begin1, end1, begin2, end2, predicate)         \
  (::picidae::lightweighttest::test_all_with_impl(                           \
      std::cerr, __FILE__, __LINE__, PICIDAE_CURRENT_FUNCTION, begin1, end1, \
      begin2, end2, predicate))

#ifndef PICIDAE_NO_EXCEPTIONS
#define PICIDAE_TEST_THROWS(EXPR, EXCEP)                              \
  try {                                                               \
    EXPR;                                                             \
    ::picidae::lightweighttest::throw_failed_impl(                    \
        #EXPR, #EXCEP, __FILE__, __LINE__, PICIDAE_CURRENT_FUNCTION); \
  } catch (EXCEP const&) {                                            \
    ::picidae::lightweighttest::test_results();                       \
  } catch (...) {                                                     \
    ::picidae::lightweighttest::throw_failed_impl(                    \
        #EXPR, #EXCEP, __FILE__, __LINE__, PICIDAE_CURRENT_FUNCTION); \
  }                                                                   \
  //
#else
#define PICIDAE_TEST_THROWS(EXPR, EXCEP)
#endif

#ifndef PICIDAE_NO_EXCEPTIONS
#define PICIDAE_TEST_NO_THROW(EXPR)                                     \
  try {                                                                 \
    EXPR;                                                               \
  } catch (const std::exception& e) {                                   \
    ::picidae::lightweighttest::no_throw_failed_impl(                   \
        #EXPR, e.what(), __FILE__, __LINE__, PICIDAE_CURRENT_FUNCTION); \
  } catch (...) {                                                       \
    ::picidae::lightweighttest::no_throw_failed_impl(                   \
        #EXPR, __FILE__, __LINE__, PICIDAE_CURRENT_FUNCTION);           \
  }
#else
#define PICIDAE_TEST_NO_THROW(EXPR) \
  { EXPR; }
#endif

PICIDAE_NAMESPACE_END(lightweighttest)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_LIGHTWEIGHTTEST_LIGHTWEIGHTTEST_HPP__
