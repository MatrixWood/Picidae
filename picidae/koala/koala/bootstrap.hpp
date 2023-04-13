#ifndef KOALA_BOOTSTRAP_HPP__
#define KOALA_BOOTSTRAP_HPP__

#include "utility.hpp"
#include "register_function.hpp"
#include "koala_impl.hpp"

namespace koala::bootstrap
{
  class BootStrap {
   private:
    static BoxedValue unknown_assign(BoxedValue lhs, BoxedValue rhs) {
      if (lhs.is_undef()) {
        return lhs.assign(rhs);
      }
    }

    static void print(const std::string& s) noexcept { fwrite(s.c_str(), 1, s.size(), stdout); }
    static void println(const std::string& s) noexcept { puts(s.c_str()); }

    static void opers_arthmetic_pod(Module& m) {

    }
  };
} // namespace koala::bootstrap


#endif
