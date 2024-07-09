//
// Created by gbian on 06/06/2024.
//

#pragma once

#include "timeFactors.hpp"

DISABLE_WARNINGS_PUSH(26447 26481)

namespace vnd {
    class TimeValues {
    public:
        TimeValues() = default;

        explicit TimeValues(const long double nanoseconds_) noexcept
          : seconds(nanoseconds_ / SECONDSFACTOR), millis(nanoseconds_ / MILLISECONDSFACTOR), micro(nanoseconds_ / MICROSECONDSFACTOR),
            nano(nanoseconds_) {}

        TimeValues(const long double seconds_, const long double millis_, const long double micro_, const long double nano_) noexcept
          : seconds(seconds_), millis(millis_), micro(micro_), nano(nano_) {}

        TimeValues(const TimeValues &other) = default;
        TimeValues(TimeValues &&other) noexcept = default;
        TimeValues &operator=(const TimeValues &other) = default;
        TimeValues &operator=(TimeValues &&other) noexcept = default;

        [[nodiscard]] const long double &get_seconds() const noexcept { return seconds; }
        [[nodiscard]] const long double &get_millis() const noexcept { return millis; }
        [[nodiscard]] const long double &get_micro() const noexcept { return micro; }
        [[nodiscard]] const long double &get_nano() const noexcept { return nano; }

    private:
        long double seconds{};
        long double millis{};
        long double micro{};
        long double nano{};
    };

    class ValueLable {
    public:
        ValueLable() noexcept = default;
        ValueLable(const long double time_val, const std::string_view time_label) noexcept : timeVal(time_val), timeLabel(time_label) {}
        ValueLable(const ValueLable &other) = default;
        ValueLable(ValueLable &&other) noexcept = default;
        ValueLable &operator=(const ValueLable &other) = default;
        ValueLable &operator=(ValueLable &&other) noexcept = default;

        [[nodiscard]] std::string transformTimeMicro(long double inputTimeMicro) const noexcept {
            using namespace std::chrono;

            const duration<long double, std::micro> durationmicros(inputTimeMicro);

            auto durationUs = duration_cast<microseconds>(durationmicros);
            auto durationNs = duration_cast<nanoseconds>(durationmicros - durationUs);

            return FORMAT("{}us,{}ns", C_LD(durationUs.count()), C_LD(durationNs.count()));
        }

        [[nodiscard]] std::string transformTimeMilli(long double inputTimeMilli) const noexcept {
            using namespace std::chrono;

            const duration<long double, std::milli> durationmils(inputTimeMilli);

            auto durationMs = duration_cast<milliseconds>(durationmils);
            auto durationUs = duration_cast<microseconds>(durationmils - durationMs);
            auto durationNs = duration_cast<nanoseconds>(durationmils - durationMs - durationUs);

            return FORMAT("{}ms,{}us,{}ns", C_LD(durationMs.count()), C_LD(durationUs.count()), C_LD(durationNs.count()));
        }

        [[nodiscard]] std::string transformTimeSeconds(long double inputTimeSeconds) const noexcept {
            using namespace std::chrono;

            const duration<long double> durationSecs(inputTimeSeconds);

            auto durationSec = duration_cast<seconds>(durationSecs);
            auto durationMs = duration_cast<milliseconds>(durationSecs - durationSec);
            auto durationUs = duration_cast<microseconds>(durationSecs - durationSec - durationMs);
            auto durationNs = duration_cast<nanoseconds>(durationSecs - durationSec - durationMs - durationUs);

            return FORMAT("{}s,{}ms,{}us,{}ns", C_LD(durationSec.count()), C_LD(durationMs.count()), C_LD(durationUs.count()),
                          C_LD(durationNs.count()));
        }
        [[nodiscard]] std::string toString() const noexcept {
            if(timeLabel == "s") { return transformTimeSeconds(timeVal); }
            if(timeLabel == "ms") { return transformTimeMilli(timeVal); }
            if(timeLabel == "us") { return transformTimeMicro(timeVal); }
            return FORMAT("{} {}", timeVal, timeLabel);
        }

    private:
        long double timeVal{};
        std::string timeLabel{""};
    };

    class Times {
    public:
        Times() = default;

        explicit Times(const long double nanoseconds_) noexcept : values(nanoseconds_) {}

        explicit Times(const TimeValues &time_values) noexcept : values(time_values) {}

        Times(const TimeValues &time_values, const std::string_view labelseconds_, const std::string_view labelmillis_,
              const std::string_view labelmicro_, std::string_view labelnano_) noexcept
          : values(time_values), labelseconds(labelseconds_), labelmillis(labelmillis_), labelmicro(labelmicro_), labelnano(labelnano_) {}

        Times(const Times &other) = default;
        Times(Times &&other) noexcept = default;
        Times &operator=(const Times &other) = default;
        Times &operator=(Times &&other) noexcept = default;

        [[nodiscard]] ValueLable getRelevantTimeframe() const noexcept {
            if(values.get_seconds() > 1) {  // seconds
                return {values.get_seconds(), labelseconds};
            } else if(values.get_millis() > 1) {  // millis
                return {values.get_millis(), labelmillis};
            } else if(values.get_micro() > 1) {  // micros
                return {values.get_micro(), labelmicro};
            } else {  // nanos
                return {values.get_nano(), labelnano};
            }
        }

    private:
        // Campi della classe
        TimeValues values{};
        std::string_view labelseconds{"s"};
        std::string_view labelmillis{"ms"};
        std::string_view labelmicro{"us"};
        std::string_view labelnano{"ns"};
    };
    DISABLE_WARNINGS_POP()
}  // namespace vnd

/**
 * This function is a formatter for CodeSourceLocation using fmt.
 * \cond
 */
// NOLINTNEXTLINE
template <> struct fmt::formatter<vnd::ValueLable> : fmt::formatter<std::string_view> {
    template <typename FormatContext> auto format(const vnd::ValueLable &val, FormatContext &ctx) {
        return fmt::formatter<std::string_view>::format(val.toString(), ctx);
    }
};
/** \endcond */
