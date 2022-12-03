#ifndef KOALA_BoxedNumber_HPP__
#define KOALA_BoxedNumber_HPP__

#include "boxed_value.hpp"
#include "type_conversions.hpp"
#include "koala_algebraic.hpp"

namespace koala
{
  namespace exception
  {
    struct arithmetic_error : std::runtime_error {
      explicit arithmetic_error(const std::string &reason)
          : std::runtime_error("Arithmetic error: " + reason) {
      }
      arithmetic_error(const arithmetic_error &) = default;
      ~arithmetic_error() noexcept override = default;
    };

    class bad_any_cast : public std::bad_cast {
    public:
      const char *what() const noexcept override { return "bad any cast"; }
    };
  } // namespace exception
  

  class BoxedNumber {
   private:
    enum class CommonTypes {
      t_int32,
      t_double,
      t_uint8,
      t_int8,
      t_uint16,
      t_int16,
      t_uint32,
      t_uint64,
      t_int64,
      t_float,
      t_long_double
    };

    template<typename T>
    constexpr static inline void check_divide_by_zero([[maybe_unused]] T t) {
      if constexpr (!std::is_floating_point<T>::value) {
        if (t == 0) {
          throw koala::exception::arithmetic_error("divide by zero");
        }
      }
    }
    
    constexpr static CommonTypes get_common_type(size_t t_size, bool t_signed) noexcept {
      return (t_size == 1 && t_signed) ? (CommonTypes::t_int8)
          : (t_size == 1)              ? (CommonTypes::t_uint8)
          : (t_size == 2 && t_signed)  ? (CommonTypes::t_int16)
          : (t_size == 2)              ? (CommonTypes::t_uint16)
          : (t_size == 4 && t_signed)  ? (CommonTypes::t_int32)
          : (t_size == 4)              ? (CommonTypes::t_uint32)
          : (t_size == 8 && t_signed)  ? (CommonTypes::t_int64)
                                       : (CommonTypes::t_uint64);
    }

    static CommonTypes get_common_type(const BoxedValue& t_bv) {
      const TypeInfo &inp_ = t_bv.get_type_info();

      if (inp_ == user_type<int>()) {
        return get_common_type(sizeof(int), true);
      } else if (inp_ == user_type<double>()) {
        return CommonTypes::t_double;
      } else if (inp_ == user_type<long double>()) {
        return CommonTypes::t_long_double;
      } else if (inp_ == user_type<float>()) {
        return CommonTypes::t_float;
      } else if (inp_ == user_type<char>()) {
        return get_common_type(sizeof(char), std::is_signed<char>::value);
      } else if (inp_ == user_type<unsigned char>()) {
        return get_common_type(sizeof(unsigned char), false);
      } else if (inp_ == user_type<unsigned int>()) {
        return get_common_type(sizeof(unsigned int), false);
      } else if (inp_ == user_type<long>()) {
        return get_common_type(sizeof(long), true);
      } else if (inp_ == user_type<long long>()) {
        return get_common_type(sizeof(long long), true);
      } else if (inp_ == user_type<unsigned long>()) {
        return get_common_type(sizeof(unsigned long), false);
      } else if (inp_ == user_type<unsigned long long>()) {
        return get_common_type(sizeof(unsigned long long), false);
      } else if (inp_ == user_type<std::int8_t>()) {
        return CommonTypes::t_int8;
      } else if (inp_ == user_type<std::int16_t>()) {
        return CommonTypes::t_int16;
      } else if (inp_ == user_type<std::int32_t>()) {
        return CommonTypes::t_int32;
      } else if (inp_ == user_type<std::int64_t>()) {
        return CommonTypes::t_int64;
      } else if (inp_ == user_type<std::uint8_t>()) {
        return CommonTypes::t_uint8;
      } else if (inp_ == user_type<std::uint16_t>()) {
        return CommonTypes::t_uint16;
      } else if (inp_ == user_type<std::uint32_t>()) {
        return CommonTypes::t_uint32;
      } else if (inp_ == user_type<std::uint64_t>()) {
        return CommonTypes::t_uint64;
      } else if (inp_ == user_type<wchar_t>()) {
        return get_common_type(sizeof(wchar_t), std::is_signed<wchar_t>::value);
      } else if (inp_ == user_type<char16_t>()) {
        return get_common_type(sizeof(char16_t), std::is_signed<char16_t>::value);
      } else if (inp_ == user_type<char32_t>()) {
        return get_common_type(sizeof(char32_t), std::is_signed<char32_t>::value);
      }
    }

    template<typename LHS, typename RHS>
    static auto go(Operators::Opers t_oper, const BoxedValue &t_bv, LHS *t_lhs, const LHS &c_lhs, const RHS &c_rhs) {
      switch (t_oper) {
        case Operators::Opers::equals:
          return const_var(c_lhs == c_rhs);
        case Operators::Opers::less_than:
          return const_var(c_lhs < c_rhs);
        case Operators::Opers::greater_than:
          return const_var(c_lhs > c_rhs);
        case Operators::Opers::less_than_equal:
          return const_var(c_lhs <= c_rhs);
        case Operators::Opers::greater_than_equal:
          return const_var(c_lhs >= c_rhs);
        case Operators::Opers::not_equal:
          return const_var(c_lhs != c_rhs);
        case Operators::Opers::sum:
          return const_var(c_lhs + c_rhs);
        case Operators::Opers::quotient:
          check_divide_by_zero(c_rhs);
          return const_var(c_lhs / c_rhs);
        case Operators::Opers::product:
          return const_var(c_lhs * c_rhs);
        case Operators::Opers::difference:
          return const_var(c_lhs - c_rhs);
        default:
          break;
      }

      if constexpr (!std::is_floating_point<LHS>::value && !std::is_floating_point<RHS>::value) {
        switch (t_oper) {
          case Operators::Opers::shift_left:
            return const_var(c_lhs << c_rhs);
          case Operators::Opers::shift_right:
            return const_var(c_lhs >> c_rhs);
          case Operators::Opers::remainder:
            check_divide_by_zero(c_rhs);
            return const_var(c_lhs % c_rhs);
          case Operators::Opers::bitwise_and:
            return const_var(c_lhs & c_rhs);
          case Operators::Opers::bitwise_or:
            return const_var(c_lhs | c_rhs);
          case Operators::Opers::bitwise_xor:
            return const_var(c_lhs ^ c_rhs);
          default:
            break;
        }
      }

      if (t_lhs) {
        switch (t_oper) {
          case Operators::Opers::assign:
            *t_lhs = c_rhs;
            return t_bv;
          case Operators::Opers::assign_product:
            *t_lhs *= c_rhs;
            return t_bv;
          case Operators::Opers::assign_sum:
            *t_lhs += c_rhs;
            return t_bv;
          case Operators::Opers::assign_quotient:
            check_divide_by_zero(c_rhs);
            *t_lhs /= c_rhs;
            return t_bv;
          case Operators::Opers::assign_difference:
            *t_lhs -= c_rhs;
            return t_bv;
          default:
            break;
        }

        if constexpr (!std::is_floating_point<LHS>::value && !std::is_floating_point<RHS>::value) {
          switch (t_oper) {
            case Operators::Opers::assign_bitwise_and:
              check_divide_by_zero(c_rhs);
              *t_lhs &= c_rhs;
              return t_bv;
            case Operators::Opers::assign_bitwise_or:
              *t_lhs |= c_rhs;
              return t_bv;
            case Operators::Opers::assign_shift_left:
              *t_lhs <<= c_rhs;
              return t_bv;
            case Operators::Opers::assign_shift_right:
              *t_lhs >>= c_rhs;
              return t_bv;
            case Operators::Opers::assign_remainder:
              *t_lhs %= c_rhs;
              return t_bv;
            case Operators::Opers::assign_bitwise_xor:
              *t_lhs ^= c_rhs;
              return t_bv;
            default:
              break;
          }
        }
      }
    }

    template<typename Callable>
    inline static auto visit(const BoxedValue &bv, Callable &&callable) {
      switch (get_common_type(bv)) {
        case CommonTypes::t_int32:
          return callable(*static_cast<const std::int32_t *>(bv.get_const_ptr()));
        case CommonTypes::t_uint8:
          return callable(*static_cast<const std::uint8_t *>(bv.get_const_ptr()));
        case CommonTypes::t_int8:
          return callable(*static_cast<const std::int8_t *>(bv.get_const_ptr()));
        case CommonTypes::t_uint16:
          return callable(*static_cast<const std::uint16_t *>(bv.get_const_ptr()));
        case CommonTypes::t_int16:
          return callable(*static_cast<const std::int16_t *>(bv.get_const_ptr()));
        case CommonTypes::t_uint32:
          return callable(*static_cast<const std::uint32_t *>(bv.get_const_ptr()));
        case CommonTypes::t_uint64:
          return callable(*static_cast<const std::uint64_t *>(bv.get_const_ptr()));
        case CommonTypes::t_int64:
          return callable(*static_cast<const std::int64_t *>(bv.get_const_ptr()));
        case CommonTypes::t_double:
          return callable(*static_cast<const double *>(bv.get_const_ptr()));
        case CommonTypes::t_float:
          return callable(*static_cast<const float *>(bv.get_const_ptr()));
        case CommonTypes::t_long_double:
          return callable(*static_cast<const long double *>(bv.get_const_ptr()));
      }
    }

    inline static BoxedValue oper(Operators::Opers t_oper, const BoxedValue &t_lhs) {
      auto unary_operator = [t_oper, &t_lhs](const auto &c_lhs) {
        auto *lhs = static_cast<std::decay_t<decltype(c_lhs)> *>(t_lhs.get_ptr());

        if (lhs) {
          switch (t_oper) {
            case Operators::Opers::pre_increment:
              ++(*lhs);
              return t_lhs;
            case Operators::Opers::pre_decrement:
              --(*lhs);
              return t_lhs;
            default:
              break;
          }
        }

        switch (t_oper) {
          case Operators::Opers::unary_minus:
            return const_var(-c_lhs);
          case Operators::Opers::unary_plus:
            return const_var(+c_lhs);
          default:
            break;
        }

        if constexpr (!std::is_floating_point_v<std::decay_t<decltype(c_lhs)>>) {
          switch (t_oper) {
            case Operators::Opers::bitwise_complement:
              return const_var(~c_lhs);
            default:
              break;
          }
        }
      };

      return visit(t_lhs, unary_operator);
    }

    inline static BoxedValue oper(Operators::Opers t_oper, const BoxedValue &t_lhs, const BoxedValue &t_rhs) {
      auto lhs_visit = [t_oper, &t_lhs, &t_rhs](const auto &c_lhs) {
        auto *lhs = t_lhs.is_return_value() ? nullptr : static_cast<std::decay_t<decltype(c_lhs)> *>(t_lhs.get_ptr());

        auto rhs_visit = [t_oper, &t_lhs, lhs, &c_lhs](const auto &c_rhs) { return go(t_oper, t_lhs, lhs, c_lhs, c_rhs); };

        return visit(t_rhs, rhs_visit);
      };

      return visit(t_lhs, lhs_visit);
    }

    template<typename Target, typename Source>
    static inline Target get_as_aux(const BoxedValue &t_bv) {
      return static_cast<Target>(*static_cast<const Source *>(t_bv.get_const_ptr()));
    }

    template<typename Source>
    static std::string to_string_aux(const BoxedValue &v) {
      std::ostringstream oss;
      oss << *static_cast<const Source *>(v.get_const_ptr());
      return oss.str();
    }

   public:
    BoxedNumber()
        : bv(BoxedValue(0)) {
    }

    explicit BoxedNumber(BoxedValue v)
        : bv(std::move(v)) {
      validate_boxed_number(bv);
    }

    BoxedNumber(const BoxedNumber &) = default;
    BoxedNumber(BoxedNumber &&) = default;
    BoxedNumber &operator=(BoxedNumber &&) = default;

    template<typename T>
    explicit BoxedNumber(T t)
        : bv(BoxedValue(t)) {
      validate_boxed_number(bv);
    }

    static BoxedValue clone(const BoxedValue &t_bv) { return BoxedNumber(t_bv).get_as(t_bv.get_type_info()).bv; }

    static bool is_floating_point(const BoxedValue &t_bv) {
      const TypeInfo &inp_ = t_bv.get_type_info();

      if (inp_ == user_type<double>()) {
        return true;
      } else if (inp_ == user_type<long double>()) {
        return true;
      } else if (inp_ == user_type<float>()) {
        return true;
      } else {
        return false;
      }
    }

    BoxedNumber get_as(const TypeInfo &inp_) const {
      if (inp_.bare_equal(user_type<int>())) {
        return BoxedNumber(get_as<int>());
      } else if (inp_.bare_equal(user_type<double>())) {
        return BoxedNumber(get_as<double>());
      } else if (inp_.bare_equal(user_type<float>())) {
        return BoxedNumber(get_as<float>());
      } else if (inp_.bare_equal(user_type<long double>())) {
        return BoxedNumber(get_as<long double>());
      } else if (inp_.bare_equal(user_type<char>())) {
        return BoxedNumber(get_as<char>());
      } else if (inp_.bare_equal(user_type<unsigned char>())) {
        return BoxedNumber(get_as<unsigned char>());
      } else if (inp_.bare_equal(user_type<wchar_t>())) {
        return BoxedNumber(get_as<wchar_t>());
      } else if (inp_.bare_equal(user_type<char16_t>())) {
        return BoxedNumber(get_as<char16_t>());
      } else if (inp_.bare_equal(user_type<char32_t>())) {
        return BoxedNumber(get_as<char32_t>());
      } else if (inp_.bare_equal(user_type<unsigned int>())) {
        return BoxedNumber(get_as<unsigned int>());
      } else if (inp_.bare_equal(user_type<long>())) {
        return BoxedNumber(get_as<long>());
      } else if (inp_.bare_equal(user_type<long long>())) {
        return BoxedNumber(get_as<long long>());
      } else if (inp_.bare_equal(user_type<unsigned long>())) {
        return BoxedNumber(get_as<unsigned long>());
      } else if (inp_.bare_equal(user_type<unsigned long long>())) {
        return BoxedNumber(get_as<unsigned long long>());
      } else if (inp_.bare_equal(user_type<int8_t>())) {
        return BoxedNumber(get_as<int8_t>());
      } else if (inp_.bare_equal(user_type<int16_t>())) {
        return BoxedNumber(get_as<int16_t>());
      } else if (inp_.bare_equal(user_type<int32_t>())) {
        return BoxedNumber(get_as<int32_t>());
      } else if (inp_.bare_equal(user_type<int64_t>())) {
        return BoxedNumber(get_as<int64_t>());
      } else if (inp_.bare_equal(user_type<uint8_t>())) {
        return BoxedNumber(get_as<uint8_t>());
      } else if (inp_.bare_equal(user_type<uint16_t>())) {
        return BoxedNumber(get_as<uint16_t>());
      } else if (inp_.bare_equal(user_type<uint32_t>())) {
        return BoxedNumber(get_as<uint32_t>());
      } else if (inp_.bare_equal(user_type<uint64_t>())) {
        return BoxedNumber(get_as<uint64_t>());
      }
    }

    template<typename Source, typename Target>
    static void check_type() {
      if (sizeof(Source) != sizeof(Target) || std::is_signed<Source>() != std::is_signed<Target>()
          || std::is_floating_point<Source>() != std::is_floating_point<Target>()) {
        throw koala::exception::bad_any_cast();
      }
    }

    template<typename Target>
    Target get_as_checked() const {
      switch (get_common_type(bv)) {
        case CommonTypes::t_int32:
          check_type<int32_t, Target>();
          return get_as_aux<Target, int32_t>(bv);
        case CommonTypes::t_uint8:
          check_type<uint8_t, Target>();
          return get_as_aux<Target, uint8_t>(bv);
        case CommonTypes::t_int8:
          check_type<int8_t, Target>();
          return get_as_aux<Target, int8_t>(bv);
        case CommonTypes::t_uint16:
          check_type<uint16_t, Target>();
          return get_as_aux<Target, uint16_t>(bv);
        case CommonTypes::t_int16:
          check_type<int16_t, Target>();
          return get_as_aux<Target, int16_t>(bv);
        case CommonTypes::t_uint32:
          check_type<uint32_t, Target>();
          return get_as_aux<Target, uint32_t>(bv);
        case CommonTypes::t_uint64:
          check_type<uint64_t, Target>();
          return get_as_aux<Target, uint64_t>(bv);
        case CommonTypes::t_int64:
          check_type<int64_t, Target>();
          return get_as_aux<Target, int64_t>(bv);
        case CommonTypes::t_double:
          check_type<double, Target>();
          return get_as_aux<Target, double>(bv);
        case CommonTypes::t_float:
          check_type<float, Target>();
          return get_as_aux<Target, float>(bv);
        case CommonTypes::t_long_double:
          check_type<long double, Target>();
          return get_as_aux<Target, long double>(bv);
      }

      throw koala::exception::bad_any_cast();
    }

    template<typename Target>
    Target get_as() const {
      switch (get_common_type(bv)) {
        case CommonTypes::t_int32:
          return get_as_aux<Target, int32_t>(bv);
        case CommonTypes::t_uint8:
          return get_as_aux<Target, uint8_t>(bv);
        case CommonTypes::t_int8:
          return get_as_aux<Target, int8_t>(bv);
        case CommonTypes::t_uint16:
          return get_as_aux<Target, uint16_t>(bv);
        case CommonTypes::t_int16:
          return get_as_aux<Target, int16_t>(bv);
        case CommonTypes::t_uint32:
          return get_as_aux<Target, uint32_t>(bv);
        case CommonTypes::t_uint64:
          return get_as_aux<Target, uint64_t>(bv);
        case CommonTypes::t_int64:
          return get_as_aux<Target, int64_t>(bv);
        case CommonTypes::t_double:
          return get_as_aux<Target, double>(bv);
        case CommonTypes::t_float:
          return get_as_aux<Target, float>(bv);
        case CommonTypes::t_long_double:
          return get_as_aux<Target, long double>(bv);
      }

      throw koala::exception::bad_any_cast();
    }

    std::string to_string() const {
      switch (get_common_type(bv)) {
        case CommonTypes::t_int32:
          return std::to_string(get_as<int32_t>());
        case CommonTypes::t_uint8:
          return std::to_string(get_as<uint32_t>());
        case CommonTypes::t_int8:
          return std::to_string(get_as<int32_t>());
        case CommonTypes::t_uint16:
          return std::to_string(get_as<uint16_t>());
        case CommonTypes::t_int16:
          return std::to_string(get_as<int16_t>());
        case CommonTypes::t_uint32:
          return std::to_string(get_as<uint32_t>());
        case CommonTypes::t_uint64:
          return std::to_string(get_as<uint64_t>());
        case CommonTypes::t_int64:
          return std::to_string(get_as<int64_t>());
        case CommonTypes::t_double:
          return to_string_aux<double>(bv);
        case CommonTypes::t_float:
          return to_string_aux<float>(bv);
        case CommonTypes::t_long_double:
          return to_string_aux<long double>(bv);
      }

      throw koala::exception::bad_any_cast();
    }

    static void validate_boxed_number(const BoxedValue &v) {
      const TypeInfo &inp_ = v.get_type_info();
      if (inp_ == user_type<bool>()) {
        throw koala::exception::bad_any_cast();
      }

      if (!inp_.is_arithmetic()) {
        throw koala::exception::bad_any_cast();
      }
    }

    static bool equals(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return boxed_cast<bool>(oper(Operators::Opers::equals, t_lhs.bv, t_rhs.bv));
    }

    static bool less_than(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return boxed_cast<bool>(oper(Operators::Opers::less_than, t_lhs.bv, t_rhs.bv));
    }

    static bool greater_than(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return boxed_cast<bool>(oper(Operators::Opers::greater_than, t_lhs.bv, t_rhs.bv));
    }

    static bool greater_than_equal(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return boxed_cast<bool>(oper(Operators::Opers::greater_than_equal, t_lhs.bv, t_rhs.bv));
    }

    static bool less_than_equal(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return boxed_cast<bool>(oper(Operators::Opers::less_than_equal, t_lhs.bv, t_rhs.bv));
    }

    static bool not_equal(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return boxed_cast<bool>(oper(Operators::Opers::not_equal, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber pre_decrement(BoxedNumber t_lhs) { return BoxedNumber(oper(Operators::Opers::pre_decrement, t_lhs.bv)); }

    static BoxedNumber pre_increment(BoxedNumber t_lhs) { return BoxedNumber(oper(Operators::Opers::pre_increment, t_lhs.bv)); }

    static const BoxedNumber sum(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::sum, t_lhs.bv, t_rhs.bv));
    }

    static const BoxedNumber unary_plus(const BoxedNumber &t_lhs) { return BoxedNumber(oper(Operators::Opers::unary_plus, t_lhs.bv)); }

    static const BoxedNumber unary_minus(const BoxedNumber &t_lhs) { return BoxedNumber(oper(Operators::Opers::unary_minus, t_lhs.bv)); }

    static const BoxedNumber difference(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::difference, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign_bitwise_and(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_bitwise_and, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign_bitwise_or(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_bitwise_or, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign_bitwise_xor(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_bitwise_xor, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign_remainder(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_remainder, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign_shift_left(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_shift_left, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign_shift_right(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_shift_right, t_lhs.bv, t_rhs.bv));
    }

    static const BoxedNumber bitwise_and(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::bitwise_and, t_lhs.bv, t_rhs.bv));
    }

    static const BoxedNumber bitwise_complement(const BoxedNumber &t_lhs) {
      return BoxedNumber(oper(Operators::Opers::bitwise_complement, t_lhs.bv, BoxedValue(0)));
    }

    static const BoxedNumber bitwise_xor(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::bitwise_xor, t_lhs.bv, t_rhs.bv));
    }

    static const BoxedNumber bitwise_or(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::bitwise_or, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign_product(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_product, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign_quotient(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_quotient, t_lhs.bv, t_rhs.bv));
    }

    static BoxedNumber assign_sum(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_sum, t_lhs.bv, t_rhs.bv));
    }
    static BoxedNumber assign_difference(BoxedNumber t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::assign_difference, t_lhs.bv, t_rhs.bv));
    }

    static const BoxedNumber quotient(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::quotient, t_lhs.bv, t_rhs.bv));
    }

    static const BoxedNumber shift_left(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::shift_left, t_lhs.bv, t_rhs.bv));
    }

    static const BoxedNumber product(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::product, t_lhs.bv, t_rhs.bv));
    }

    static const BoxedNumber remainder(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::remainder, t_lhs.bv, t_rhs.bv));
    }

    static const BoxedNumber shift_right(const BoxedNumber &t_lhs, const BoxedNumber &t_rhs) {
      return BoxedNumber(oper(Operators::Opers::shift_right, t_lhs.bv, t_rhs.bv));
    }

    static BoxedValue do_oper(Operators::Opers t_oper, const BoxedValue &t_lhs, const BoxedValue &t_rhs) {
      return oper(t_oper, t_lhs, t_rhs);
    }

    static BoxedValue do_oper(Operators::Opers t_oper, const BoxedValue &t_lhs) { return oper(t_oper, t_lhs); }

    BoxedValue bv;
  };
} // namespace koala

#endif
