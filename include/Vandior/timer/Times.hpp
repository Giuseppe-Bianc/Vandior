//
// Created by gbian on 06/06/2024.
//

#pragma once

#include "../headers.hpp"
#include "timeFactors.hpp"

namespace vnd {

    class Times {
    public:
        // Campi della classe
        long double seconds{};
        long double millis{};
        long double micro{};
        long double nano{};
        std::string labelseconds{};
        std::string labelmillis{};
        std::string labelmicro{};
        std::string labelnano{};

        Times() = default;

        explicit Times(const long double nanoseconds)
          : seconds(nanoseconds / SECONDSFACTOR), millis(nanoseconds / MILLISECONDSFACTOR), micro(nanoseconds / MICROSECONDSFACTOR),
            nano(nanoseconds), labelseconds("s"), labelmillis("ms"), labelmicro("us"), labelnano("ns") {}

        Times(const long double seconds, const long double millis, const long double micro, const long double nano)
          : seconds(seconds), millis(millis), micro(micro), nano(nano), labelseconds("s"), labelmillis("ms"), labelmicro("us"),
            labelnano("ns") {}

        Times(const long double seconds, const long double millis, const long double micro, const long double nano,
              const std::string &labelseconds, const std::string &labelmillis, const std::string &labelmicro, const std::string &labelnano)
          : seconds(seconds), millis(millis), micro(micro), nano(nano), labelseconds(labelseconds), labelmillis(labelmillis),
            labelmicro(labelmicro), labelnano(labelnano) {}
        Times(const Times &other) = default;
        Times(Times &&other) noexcept = default;
        Times &operator=(const Times &other) = default;
        Times &operator=(Times &&other) noexcept = default;
        [[nodiscard]] std::pair<long double, std::string> getRelevantTimeframe() const noexcept {
            if(seconds > 1) [[likely]] {  // seconds
                return {seconds, labelseconds};
            } else if(millis > 1) [[likely]] {  // milli
                return {millis, labelmillis};
            } else if(micro > 1) [[likely]] {  // micro
                return {micro, labelmicro};
            } else [[unlikely]] {
                return {nano, labelnano};
            }
        }
    };
}  // namespace vnd
