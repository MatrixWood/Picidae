#ifndef KOALA_BOXED_CAST_HPP__
#define KOALA_BOXED_CAST_HPP__

#include <memory>
#include <type_traits>

#include "boxed_value.hpp"
#include "type_info.hpp"

namespace koala
{
  namespace exception
  {
    class bad_boxed_cast : public std::bad_cast {

    };
  } // namespace exception
  
} // namespace koala

#endif
