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

template <class Duration>
using sys_time = std::chrono::time_point<std::chrono::system_clock, Duration>;

using sys_days = sys_time<days>;
using sys_seconds = sys_time<std::chrono::seconds>;

struct local_t {};

template <class Duration>
using local_time = std::chrono::time_point<local_t, Duration>;

using local_seconds = local_time<std::chrono::seconds>;
using local_days = local_time<days>;

PICIDAE_NAMESPACE_END(date)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_DATE_DETAIL_COMMON_HPP__