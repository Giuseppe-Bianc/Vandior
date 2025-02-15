// NOLINTBEGIN(*-include-cleaner, *-identifier-length, *-special-member-functions, *-convert-member-functions-to-static)
#pragma once

#include "timeFactors.hpp"

DISABLE_WARNINGS_PUSH(26447 26481)  // NOLINT(*-avoid-non-const-global-variables, *-avoid-magic-numbers, *-magic-numbers)

namespace vnd {
    class TimeValues {
    public:
        constexpr TimeValues() noexcept = default;

        explicit constexpr TimeValues(const long double nanoseconds_) noexcept
          : seconds(nanoseconds_ / SECONDSFACTOR), millis(nanoseconds_ / MILLISECONDSFACTOR), micro(nanoseconds_ / MICROSECONDSFACTOR),
            nano(nanoseconds_) {}

        // NOLINTNEXTLINE(*-easily-swappable-parameters)
        constexpr TimeValues(long double seconds_, long double millis_, long double micro_, long double nano_) noexcept
          : seconds(seconds_), millis(millis_), micro(micro_), nano(nano_) {}

        TimeValues(const TimeValues &other) noexcept = default;
        TimeValues(TimeValues &&other) noexcept = default;
        TimeValues &operator=(const TimeValues &other) noexcept = default;
        TimeValues &operator=(TimeValues &&other) noexcept = default;

        [[nodiscard]] constexpr long double get_seconds() const noexcept { return seconds; }
        [[nodiscard]] constexpr long double get_millis() const noexcept { return millis; }
        [[nodiscard]] constexpr long double get_micro() const noexcept { return micro; }
        [[nodiscard]] constexpr long double get_nano() const noexcept { return nano; }

    private:
        long double seconds{};
        long double millis{};
        long double micro{};
        long double nano{};
    };

    class ValueLabel {
    public:
        constexpr ValueLabel() noexcept = default;
        // NOLINTNEXTLINE(*-easily-swappable-parameters)
        constexpr ValueLabel(const long double time_val, std::string_view time_label) noexcept : timeVal(time_val), timeLabel(time_label) {}
        ValueLabel(const ValueLabel &other) noexcept = default;
        ValueLabel(ValueLabel &&other) noexcept = default;
        ValueLabel &operator=(const ValueLabel &other) noexcept = default;
        ValueLabel &operator=(ValueLabel &&other) noexcept = default;

        [[nodiscard]] std::string transformTimeMicro(const long double inputTimeMicro) const noexcept {
            using namespace std::chrono;

            const auto durationmicros = microlld(inputTimeMicro);

            const auto durationUs = duration_cast<microseconds>(durationmicros);
            const auto durationNs = round<nanoseconds>(durationmicros - durationUs);

            return FORMAT("{}us,{}ns", C_LD(durationUs.count()), C_LD(durationNs.count()));
        }

        [[nodiscard]] std::string transformTimeMilli(const long double inputTimeMilli) const noexcept {
            using namespace std::chrono;

            const auto durationmils = millilld(inputTimeMilli);

            const auto durationMs = duration_cast<milliseconds>(durationmils);
            const auto durationUs = round<microseconds>(durationmils - durationMs);
            const auto durationNs = round<nanoseconds>(durationmils - durationMs - durationUs);

            return FORMAT("{}ms,{}us,{}ns", C_LD(durationMs.count()), C_LD(durationUs.count()), C_LD(durationNs.count()));
        }

        [[nodiscard]] std::string transformTimeSeconds(const long double inputTimeSeconds) const noexcept {
            using namespace std::chrono;

            const auto durationSecs = seclld(inputTimeSeconds);

            const auto durationSec = duration_cast<seconds>(durationSecs);
            const auto durationMs = round<milliseconds>(durationSecs - durationSec);
            const auto durationUs = round<microseconds>(durationSecs - durationSec - durationMs);
            const auto durationNs = round<nanoseconds>(durationSecs - durationSec - durationMs - durationUs);

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
        std::string_view timeLabel{""};
    };

    class Times {
    public:
        Times() noexcept = default;

        explicit Times(const long double nanoseconds_) noexcept : values(nanoseconds_) {}

        explicit Times(const TimeValues &time_values) noexcept : values(time_values) {}

        // NOLINTNEXTLINE(*-easily-swappable-parameters)
        Times(const TimeValues &time_values, std::string_view labelseconds_, std::string_view labelmillis_, std::string_view labelmicro_,
              std::string_view labelnano_) noexcept
          : values(time_values), labelseconds(labelseconds_), labelmillis(labelmillis_), labelmicro(labelmicro_), labelnano(labelnano_) {}

        Times(const Times &other) noexcept = default;
        Times(Times &&other) noexcept = default;
        Times &operator=(const Times &other) noexcept = default;
        Times &operator=(Times &&other) noexcept = default;

        [[nodiscard]] ValueLabel getRelevantTimeframe() const noexcept {
            const auto seconds = values.get_seconds();
            const auto millis = values.get_millis();
            const auto micro = values.get_micro();

            if(seconds > 1.0L) {  // seconds NOLINT(*-branch-clone)
                return {seconds, labelseconds};
            } else if(millis > 1.0L) {  // millis
                return {millis, labelmillis};
            } else if(micro > 1.0L) {  // micros
                return {micro, labelmicro};
            }
            return {values.get_nano(), labelnano};  // nanos
        }

    private:
        TimeValues values;
        std::string_view labelseconds{"s"};
        std::string_view labelmillis{"ms"};
        std::string_view labelmicro{"us"};
        std::string_view labelnano{"ns"};
    };
    DISABLE_WARNINGS_POP()
}  // namespace vnd

/**
 * This function is a formatter for Token using fmt.
 * \cond
 */
template <> struct fmt::formatter<vnd::ValueLabel> : fmt::formatter<std::string_view> {
    auto format(const vnd::ValueLabel &val, format_context &ctx) const -> format_context::iterator {
        return fmt::formatter<std::string_view>::format(val.toString(), ctx);
    }
};
/** \endcond */

// NOLINTEND(*-include-cleaner, *-identifier-length, *-special-member-functions, *-convert-member-functions-to-static)