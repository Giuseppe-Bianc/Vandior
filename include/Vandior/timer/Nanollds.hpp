//
// Created by gbian on 04/06/2024.
//

#pragma once

#include "Clocks.hpp"
#include "cronoRedefinition.hpp"

namespace vnd {

    class Nanollds {
    public:
        nanolld elapsed;
        nanolld utc_elapsed;
        nanolld tai_elapsed;
        nanolld gps_elapsed;
        nanolld file_elapsed;
        Nanollds() = default;
        [[nodiscard]] Nanollds(const nanolld &elapsed, const nanolld &utc_elapsed, const nanolld &tai_elapsed, const nanolld &gps_elapsed,
                               const nanolld &file_elapsed)
          : elapsed(elapsed), utc_elapsed(utc_elapsed), tai_elapsed(tai_elapsed), gps_elapsed(gps_elapsed), file_elapsed(file_elapsed) {}

        Clocks toClocks() const {
            return Clocks{elapsed.count(), utc_elapsed.count(), tai_elapsed.count(), gps_elapsed.count(), file_elapsed.count()};
        }
    };

}  // namespace vnd
