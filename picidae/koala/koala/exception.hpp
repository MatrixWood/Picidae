#ifndef KOALA_EXCEPTION_HPP__
#define KOALA_EXCEPTION_HPP__

#include <exception>

#include "boxed_number.hpp"

namespace koala {
namespace exception {
class bad_any_cast : public std::bad_cast {
 public:
  const char* what() const noexcept override { return "[Koala] bad any cast"; }
};


}  // namespace exception
}  // namespace koala

#endif  // KOALA_EXCEPTION_HPP__