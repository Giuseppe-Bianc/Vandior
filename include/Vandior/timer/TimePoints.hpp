//
// Created by gbian on 04/06/2024.
//

#pragma once

#include "Nanollds.hpp"
#include "cronoRedefinition.hpp"

namespace vnd {

    class TimePoints {
    public:
        time_point time_p;
        utc_time_point utc_time_p;
        tai_time_point tai_time_p;
        gps_time_point gps_time_p;
        file_time_point file_time_p;

        TimePoints() = default;

        TimePoints(const time_point &time_p, const utc_time_point &utc_time_p, const tai_time_point &tai_time_p,
                   const gps_time_point &gps_time_p, const file_time_point &file_time_p) noexcept
          : time_p(time_p), utc_time_p(utc_time_p), tai_time_p(tai_time_p), gps_time_p(gps_time_p), file_time_p(file_time_p) {}
        const Nanollds operator-(const TimePoints &other) const noexcept {
            return Nanollds{this->time_p - other.time_p, this->utc_time_p - other.utc_time_p, this->tai_time_p - other.tai_time_p,
                            this->gps_time_p - other.gps_time_p, this->file_time_p - other.file_time_p};
        }
    };
}  // namespace vnd