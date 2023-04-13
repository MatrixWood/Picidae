#ifndef KOALA_REGISTER_FUCNTION_HPP__
#define KOALA_REGISTER_FUNCTION_HPP__

#include <type_traits>

#include "bind_first.hpp"
#include "function_signature.hpp"
#include "proxy_function.hpp"

namespace koala
{
  template <typename T>
  ProxyFunction func(T&& t) {
    return ProxyFunction();
  }

} // namespace koala


#endif
