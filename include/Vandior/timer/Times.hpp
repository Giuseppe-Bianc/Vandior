//
// Created by gbian on 06/06/2024.
//

#pragma once

// #include "../headers.hpp"
#include "timeFactors.hpp"

namespace vnd {
    class TimeValues {
    public:
        long double seconds{};
        long double millis{};
        long double micro{};
        long double nano{};

        TimeValues() = default;
        explicit TimeValues(const long double nanoseconds)
          : seconds(nanoseconds / SECONDSFACTOR), millis(nanoseconds / MILLISECONDSFACTOR), micro(nanoseconds / MICROSECONDSFACTOR),
            nano(nanoseconds) {}

        TimeValues(const long double seconds, const long double millis, const long double micro, const long double nano)
          : seconds(seconds), millis(millis), micro(micro), nano(nano) {}

        TimeValues(const TimeValues &other) = default;
        TimeValues(TimeValues &&other) noexcept = default;
        TimeValues &operator=(const TimeValues &other) = default;
        TimeValues &operator=(TimeValues &&other) noexcept = default;
    };

    class Times {
    public:
        // Campi della classe
        TimeValues values{};
        std::string labelseconds{"s"};
        std::string labelmillis{"ms"};
        std::string labelmicro{"us"};
        std::string labelnano{"ns"};

        Times() = default;

        explicit Times(const long double nanoseconds) : values(nanoseconds) {}

        explicit Times(const TimeValues &time_values) : values(time_values) {}

        Times(const TimeValues &time_values, const std::string &labelseconds, const std::string &labelmillis, const std::string &labelmicro,
              const std::string &labelnano)
          : values(time_values), labelseconds(labelseconds), labelmillis(labelmillis), labelmicro(labelmicro), labelnano(labelnano) {}
        Times(const Times &other) = default;
        Times(Times &&other) noexcept = default;
        Times &operator=(const Times &other) = default;
        Times &operator=(Times &&other) noexcept = default;
        [[nodiscard]] std::pair<long double, std::string> getRelevantTimeframe() const noexcept {
            if(values.seconds > 1) {  // seconds
                return {values.seconds, labelseconds};
            } else if(values.millis > 1) {  // millis
                return {values.millis, labelmillis};
            } else if(values.micro > 1) {  // micros
                return {values.micro, labelmicro};
            } else {  // nanos
                return {values.nano, labelnano};
            }
        }
    };
}  // namespace vnd