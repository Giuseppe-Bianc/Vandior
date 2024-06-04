//
// Created by gbian on 04/06/2024.
//

#pragma once

#include "timeFactors.hpp"

class Clocks {
public:
    long double time;
    long double utc_time;
    long double tai_time;
    long double gps_time;
    long double file_time;

    // Costruttore di default
    Clocks() = default;

    // Costruttore parametrizzato
    Clocks(long double t, long double utc, long double tai, long double gps, long double file)
      : time(t), utc_time(utc), tai_time(tai), gps_time(gps), file_time(file) {}

    [[nodiscard]] Clocks getInSeconds() const noexcept {
        return {time / vnd::SECONDSFACTOR, utc_time / vnd::SECONDSFACTOR, tai_time / vnd::SECONDSFACTOR, gps_time / vnd::SECONDSFACTOR,
                file_time / vnd::SECONDSFACTOR};
    }

    [[nodiscard]] Clocks getinMillis() const noexcept {
        return {time / vnd::MILLISECONDSFACTOR, utc_time / vnd::MILLISECONDSFACTOR, tai_time / vnd::MILLISECONDSFACTOR,
                gps_time / vnd::MILLISECONDSFACTOR, file_time / vnd::MILLISECONDSFACTOR};
    }
    [[nodiscard]] Clocks getinMicros() const noexcept {
        return {time / vnd::MICROSECONDSFACTOR, utc_time / vnd::MICROSECONDSFACTOR, tai_time / vnd::MICROSECONDSFACTOR,
                gps_time / vnd::MICROSECONDSFACTOR, file_time / vnd::MICROSECONDSFACTOR};
    }
};