// NOLINTBEGIN(*-include-cleaner, *-identifier-length, *-special-member-functions)
#pragma once
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
#include "../Log.hpp"
#include "../disableWarn.hpp"
#include "../format.hpp"
#include "../headersCore.hpp"
#include "TimerConstats.hpp"
#include "Times.hpp"
#include "timeFactors.hpp"
// On GCC < 4.8, the following define is often missing. Since
// this library only uses sleep_for, this should be safe
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5 && __GNUC_MINOR__ < 8
#define _GLIBCXX_USE_NANOSLEEP
#endif

namespace vnd {
    DISABLE_WARNINGS_PUSH(26447 26455)

    /**
     * @brief Timer class for measuring the execution time of code.
     */
    class Timer {
    protected:
        using time_print_t = std::function<std::string(std::string, std::size_t, ValueLabel)>;

        std::string title_;
        std::size_t title_lenpadd;
        time_print_t time_print_;
        time_point start_;
        std::size_t cycles{1};

    public:
        /// Standard print functions
        /**
         * @brief Default print function for Timer class.
         */
        static std::string Simple(const std::string &title, [[maybe_unused]] std::size_t title_lenpadd, const ValueLabel &time) {
            return FORMAT(simpleFormat, title, time);
        }

        /**
         * @brief A more elaborate print function for Timer class.
         */
        static std::string Big(const std::string &title, std::size_t title_lenpadd, const ValueLabel &time) {
            const auto times = FORMAT(bigTimesFormat, time);
            const auto tot_len = title_lenpadd + times.length() + 3;
            const auto title_time_section = FORMAT(bigTitleTimeFormat, title, title_lenpadd - 4, times, times.length() + 1);
            return FORMAT(bigFormat, "", tot_len, title_time_section);
        }

        /**
         * @brief A compact print function for Timer class.
         */
        static std::string Compact(const std::string &title, [[maybe_unused]] std::size_t title_lenpadd, const ValueLabel &time) {
            return FORMAT(compactFormat, title, time);
        }

        /**
         * @brief A detailed print function for Timer class.
         */
        static std::string Detailed(const std::string &title, [[maybe_unused]] std::size_t title_lenpadd, const ValueLabel &time) {
            return FORMAT(detailedFormat, title, time);
        }

        static std::string createPattern(const std::size_t title_lenpadd) {
            const auto ntlenpadd = title_lenpadd / 4;
            return FORMAT(blockPatternFormat, "*", ntlenpadd);
        }
        /**
         * @brief A block style print function for Timer class.
         */
        static std::string Block(const std::string &title, std::size_t title_lenpadd, const ValueLabel &time) {
            const auto patternf = createPattern(title_lenpadd);
            const auto times = FORMAT(blockTimesFormat, time);
            return FORMAT(blockFormat, patternf, title_lenpadd, title, times);
        }
        /**
         * @brief A minimal print function for Timer class.
         */
        static std::string Minimal(const std::string &title, const std::string &time) { return FORMAT(minimalFormat, title, time); }

        /**
         * @brief Standard constructor for Timer class.
         *  Standard constructor, can set title and print function
         */
        explicit Timer(const std::string &title = "Timer", const time_print_t &time_print = Simple)
          : title_(title), title_lenpadd(title.length() + TILEPADDING), time_print_(time_print), start_(clock::now()) {}

        Timer(const Timer &) = delete;
        Timer &operator=(const Timer &) = delete;
        Timer(Timer &&) = delete;
        Timer &operator=(Timer &&) = delete;

        [[nodiscard]] std::string time_it(const std::function<void()> &f, long double target_time = 1) {
            const time_point start = start_;
            [[maybe_unused]] auto total_time = std::numeric_limits<long double>::quiet_NaN();

            start_ = clock::now();
            std::size_t n = 0;
            do {  // NOLINT(*-avoid-do-while)
                f();
                total_time = ch::duration_cast<nanolld>(clock::now() - start_).count();
            } while(n++ < MFACTOR && total_time < target_time);
            const auto total_timef = C_LD(total_time / C_LD(n));
            std::string out = FORMAT(timeItFormat, make_time_str(total_timef), std::to_string(n));
            start_ = start;
            return out;
        }

        /**
         * @brief Get the elapsed time in seconds.
         * @return Elapsed time in seconds.
         */
        [[nodiscard]] long double make_time() const noexcept { return ch::duration_cast<nanolld>(clock::now() - start_).count(); }

        /**
         * @brief Get the named times (seconds, milliseconds, microseconds, nanoseconds).
         * @param time The time in nanoseconds.
         * @return A tuple containing named times.
         */
        [[nodiscard]] static Times make_named_times(const long double time) noexcept { return Times{time}; }

        [[maybe_unused]] [[nodiscard]] Times multi_time() const noexcept { return Times{make_time()}; }

        /**
         * @brief Format the numerical value for the time string.
         * This formats the numerical value for the time string
         * @return A formatted time string.
         */
        [[nodiscard]] ValueLabel make_time_str() const noexcept {
            const auto time = C_LD(make_time() / C_LD(cycles));
            return make_time_str(time);
        }

        //   LCOV_EXCL_START
        /**
         * @brief Format a given time value into a string.
         * This prints out a time string from a time
         * @param time The time value in nanoseconds.
         * @return A formatted time string.
         */
        [[nodiscard]] static ValueLabel make_time_str(const long double time) noexcept {
            return make_named_times(time).getRelevantTimeframe();
        }
        // LCOV_EXCL_STOP

        /**
         * @brief Get a string representation of the Timer.
         * @return A string representation of the Timer.
         */
        [[nodiscard]] std::string to_string() const noexcept {
            const auto time = make_time_str();
            return std::invoke(time_print_, title_, title_lenpadd, time);
        }

        Timer &operator/(std::size_t val) noexcept {
            cycles = val;
            return *this;
        }
    };

    class AutoTimer : public Timer {
    public:
        using Timer::Timer;

        AutoTimer(const AutoTimer &) = delete;
        AutoTimer &operator=(const AutoTimer &) = delete;
        AutoTimer(AutoTimer &&) = delete;
        AutoTimer &operator=(AutoTimer &&) = delete;

        ~AutoTimer() noexcept {
            try {
                LINFO(to_string());
            } catch(...) {  // NOLINT(*-empty-catch)
                // Handle or log the exception as needed
            }
        }
    };
}  // namespace vnd

/**
 * This function is a formatter for Token using fmt.
 * \cond
 */
template <> struct fmt::formatter<vnd::Timer> : formatter<std::string_view> {
    auto format(const vnd::Timer &timer, format_context &ctx) const -> format_context::iterator {
        return formatter<std::string_view>::format(timer.to_string(), ctx);
    }
};
/** \endcond */

inline std::ostream &operator<<(std::ostream &os, const vnd::Timer &timer) { return os << timer.to_string(); }

DISABLE_WARNINGS_POP()
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic pop
#endif

// NOLINTEND(*-include-cleaner, *-identifier-length, *-special-member-functions)
