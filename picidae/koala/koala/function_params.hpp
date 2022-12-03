#ifndef KOALA_FUNCTION_PARAMS_HPP__
#define KOALA_FUNCTION_PARAMS_HPP__

#include "boxed_value.hpp"

namespace koala
{
  class FunctionParams {
   public:
    constexpr FunctionParams(const BoxedValue* const t_begin, const BoxedValue* const t_end)
      : _begin(t_begin),
        _end(t_end)
    { }

    constexpr FunctionParams(const BoxedValue& bv)
      : _begin(&bv),
        _end(_begin + 1)
    { }

    constexpr FunctionParams(const std::vector<BoxedValue>& vec)
      : _begin(vec.empty() ? nullptr : &vec.front()),
        _end(vec.empty() ? nullptr : &vec.front() + vec.size())
    { }

    template <size_t Size>
    constexpr explicit FunctionParams(const std::array<BoxedValue, Size>& a)
      : _begin(&a.front()),
        _end(&a.front() + Size)
    { }

    constexpr FunctionParams(const std::array<BoxedValue, size_t{0}>&)
      : _begin(nullptr),
        _end(nullptr)
    { }

    [[nodiscard]] constexpr const BoxedValue& operator [] (const std::size_t t_i) const noexcept {
      return _begin[t_i];
    }

    [[nodiscard]] constexpr const BoxedValue* begin() const noexcept { return _begin; }

    [[nodiscard]] constexpr const BoxedValue& front() const noexcept { return *_begin; }

    [[nodiscard]] constexpr const BoxedValue* end() const noexcept { return _end; }

    [[nodiscard]] constexpr std::size_t size() const noexcept { return std::size_t(_end - _begin); }

    [[nodiscard]] std::vector<BoxedValue> to_vector() const { return std::vector<BoxedValue>{_begin, _end}; }

    [[nodiscard]] constexpr bool empty() const noexcept { return _begin == _end; }

   private:
    const BoxedValue* _begin = nullptr;
    const BoxedValue* _end = nullptr;
  };
} // namespace koala

#endif
