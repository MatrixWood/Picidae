#ifndef KOALA_HANDLE_RETURN_HPP__
#define KOALA_HANDLE_RETURN_HPP__

#include <functional>
#include <memory>
#include <type_traits>

#include "boxed_number.hpp"
#include "boxed_value.hpp"

namespace koala
{
  template <class T, class U>
  class ProxyFunctionCallbaleImpl;

  template <class T>
  class AssignableProxyFunctionImpl;

  namespace detail
  {
    template <typename Ret>
    struct HandleReturn {

    };
  } // namespace detail
  
  

} // namespace koala



#endif
