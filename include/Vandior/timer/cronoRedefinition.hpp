//
// Created by gbian on 04/06/2024.
//

#pragma once

#include "../headers.hpp"

namespace vnd {
    using clock = std::chrono::high_resolution_clock;
    using utc_clock = std::chrono::utc_clock;
    using tai_clock = std::chrono::tai_clock;
    using gps_clock = std::chrono::gps_clock;
    using file_clock = std::chrono::file_clock;
    using time_point = std::chrono::time_point<clock>;
    using utc_time_point = std::chrono::time_point<utc_clock>;
    using tai_time_point = std::chrono::time_point<tai_clock>;
    using gps_time_point = std::chrono::time_point<gps_clock>;
    using file_time_point = std::chrono::time_point<file_clock>;
    using nanolld = std::chrono::duration<long double, std::nano>;

}  // namespace vnd