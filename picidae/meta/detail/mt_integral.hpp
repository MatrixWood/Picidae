#ifndef PICIDAE_DETAIL_MT_INTEGRAL_HPP__
#define PICIDAE_DETAIL_MT_INTEGRAL_HPP__

#include <type_traits>

namespace picidae {

namespace meta {

template <bool B>
using mt_bool = std::integral_constant<bool, B>;

using mt_true = mt_bool<true>;
using mt_false = mt_bool<false>;

template <class T>
using mt_to_bool = mt_bool<static_cast<bool>(T::value)>;

template <class T>
using mt_not = mt_bool<!(T::value)>;

template <int I>
using mt_int = std::integral_constant<int, I>;

template <std::size_t N>
using mt_size_t = std::integral_constant<std::size_t, N>;

}  // namespace meta

}  // namespace picidae

#endif  // PICIDAE_DETAIL_MT_INTEGRAL_HPP__
