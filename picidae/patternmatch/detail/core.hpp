#ifndef PICIDAE_PATTERNMATCH_DETAIL_CORE_HPP__
#define PICIDAE_PATTERNMATCH_DETAIL_CORE_HPP__

#include <algorithm>
#include <cstdint>
#include <tuple>
#include <utility>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(patternmatch)

PICIDAE_NAMESPACE_BEGIN(detail)

template <typename Value, bool Ref>
class ValueType {
 public:
  using ValueT = Value;
};

template <typename Value>
class ValueType<Value, true> {
 public:
  using ValueT = Value&&;
};

template <typename Value, typename... Patterns>
constexpr auto match_patterns(Value&& value, const Patterns&&... patterns);

template <typename Value, bool Ref>
class MatchHelper {
 public:
  template <typename V>
  constexpr explicit MatchHelper(V&& value) : _value{std::forward<V>(value)} {}

  template <typename... PatternPair>
  constexpr auto operator () (const PatternPair&... patterns) {
    return match_patterns(std::forward<ValueRefT>(_value), patterns...);
  }

 private:
  using ValueT = typename ValueType<Value, Ref>::ValueT;
  ValueT _value;
  using ValueRefT = ValueT&&;
};



PICIDAE_NAMESPACE_END(detail)

PICIDAE_NAMESPACE_END(patternmatch)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_PATTERNMATCH_DETAIL_CORE_HPP__
