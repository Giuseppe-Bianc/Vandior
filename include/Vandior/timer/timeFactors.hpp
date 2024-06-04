//
// Created by gbian on 04/06/2024.
//

#pragma once

namespace vnd {
    /**
     * @brief A factor for converting microseconds to seconds.
     *
     * This constant represents the conversion factor from microseconds to seconds.
     * It is used to convert time values from microseconds to seconds by multiplying
     * the time value with this factor.
     */
    inline static constexpr long double MICROSECONDSFACTOR = 1000.0L;

    /**
     * @brief A factor for converting milliseconds to seconds.
     *
     * This constant represents the conversion factor from milliseconds to seconds.
     * It is used to convert time values from milliseconds to seconds by multiplying
     * the time value with this factor.
     */
    inline static constexpr long double MILLISECONDSFACTOR = 1'000'000.0L;

    /**
     * @brief A factor for converting seconds to seconds (1 billion nanoseconds).
     *
     * This constant represents the conversion factor from seconds to seconds, specifically
     * in the unit of nanoseconds. It is used to convert time values from seconds to
     * nanoseconds by multiplying the time value with this factor.
     */
    inline static constexpr long double SECONDSFACTOR = 1'000'000'000.0L;

    /**
     * @brief A multiplier factor used in the Timer class.
     *
     * This constant represents a multiplier factor used internally in the Timer class
     * for certain calculations or operations related to time. Its specific purpose
     * may vary depending on the implementation within the Timer class.
     */
    inline static constexpr long MFACTOR = 100;
}  // namespace vnd