#ifndef PICIDAE_DATE_DETAIL_COMMON_HPP__
#define PICIDAE_DATE_DETAIL_COMMON_HPP__

#include <chrono>

#include "picidae/meta/macro.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(date)

PICIDAE_NAMESPACE_BEGIN(detail)

template <typename R1, typename R2>
using ratio_multiply = decltype(std::ratio_multiply<R1, R2>{});

template <typename R1, typename R2>
using ratio_divide = decltype(std::ratio_divide<R1, R2>{});

PICIDAE_NAMESPACE_END(detail)

using days = std::chrono::duration<
    int, detail::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

using weeks =
    std::chrono::duration<int,
                          detail::ratio_multiply<std::ratio<7>, days::period>>;

using years = std::chrono::duration<
    int, detail::ratio_multiply<std::ratio<146097, 400>, days::period>>;

using months =
    std::chrono::duration<int,
                          detail::ratio_divide<years::period, std::ratio<12>>>;

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_COMMON_HPP__