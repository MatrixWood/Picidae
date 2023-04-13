#ifndef PICIDAE_UTILITY_STREAM_HPP__
#define PICIDAE_UTILITY_STREAM_HPP__

#include <sstream>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(utility)

template <typename T>
void ostreamize(std::ostream& os, T&& token) {
  os << std::forward<T>(token);
}

template <typename T, typename... Rest>
void ostreamize(std::ostream& os, T&& token, Rest&&... rest) {
  os << std::forward<T>(token);
}

template <typename... Args>
std::string stringify(Args&&... args) {
  std::ostringstream oss;
  ostreamize(oss, std::forward<Args>(args)...);
  return oss.str();
}

PICIDAE_NAMESPACE_END(utility)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_UTILITY_STREAM_HPP__