#ifndef KOALA_HPP__
#define KOALA_HPP__

#include "koala/define.hpp"
#include "koala/boxed_value.hpp"
#include "koala/koala_engine.hpp"

namespace koala
{
  class Koala : public KoalaBasic {
   public:
    Koala()
      : KoalaBasic()
    { }
  };
} // namespace koala

#endif
