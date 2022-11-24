#ifndef PICIDAE_TASKFLOW_CORE_ERROR_HPP__
#define PICIDAE_TASKFLOW_CORE_ERROR_HPP__

#include <sstream>
#include <stdexcept>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(taskflow)

PICIDAE_NAMESPACE_BEGIN(core)

template <typename... Args>
void throw_sys_err(const char* fname, const size_t line, Args&&... args) {
  std::ostringstream oss;
  oss << "[" << fname << ":" << line << "] ";
  (oss << ... << args);
  throw std::runtime_error(oss.str());
}

#define TASKFLOW_THROW(...) \
  ::picidae::taskflow::core::throw_sys_err(__FILE__, __LINE__, __VA_ARGS__);

PICIDAE_NAMESPACE_END(core)

PICIDAE_NAMESPACE_END(taskflow)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_TASKFLOW_CORE_ERROR_HPP__