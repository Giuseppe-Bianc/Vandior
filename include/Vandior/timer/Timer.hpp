// NOLINTBEGIN(*-include-cleaner, *-identifier-length, *-special-member-functions)
#pragma once
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
#include "../Log.hpp"
#include "../disableWarn.hpp"
#include "../format.hpp"
#include "../headers.hpp"
#include "Times.hpp"
#include "timeFactors.hpp"
// On GCC < 4.8, the following define is often missing. Since
// this library only uses sleep_for, this should be safe
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5 && __GNUC_MINOR__ < 8
#define _GLIBCXX_USE_NANOSLEEP
#endif

namespace vnd {
    static inline constexpr auto simpleFormat = "{}: Time = {}";
    static inline constexpr auto bigTimesFormat = "Time = {}";
    static inline constexpr auto bigTitleTimeFormat = "|{0: ^{1}}|{2: ^{3}}|";
    static inline constexpr auto bigFormat = "\n{0:-^{1}}\n{2}\n{0:-^{1}}";
    static inline constexpr auto compactFormat = "[{}]{}";
    static inline constexpr auto detailedFormat = "Timer '{}' measured a duration of {}";
    static inline constexpr auto blockPatternFormat = "{0:=^{1}}|{0:=^{1}}|{0:=^{1}}|{0:=^{1}}";
    static inline constexpr auto blockTimesFormat = "Time:{}";
    static inline constexpr auto blockFormat = "\n{0}\n{2: ^{1}}\n{0}\n{3: ^{1}}\n{0}";
    static inline constexpr auto minimalFormat = "{} - {}";
    static inline constexpr auto timeItFormat = "{} for {} tries";
    DISABLE_WARNINGS_PUSH(6005 26447 26455 26496)

    /**
     * @brief Timer class for measuring the execution time of code.
     */
    class Timer {
    protected:
        using clock = std::chrono::high_resolution_clock;
        using time_point = std::chrono::time_point<clock>;
        using time_print_t = std::function<std::string(std::string, std::size_t, ValueLabel)>;
        using nanolld = std::chrono::duration<long double, std::nano>;

        std::string title_;
        std::size_t title_lenpadd;
        time_print_t time_print_;
        time_point start_;
        std::size_t cycles{1};

    public:
        /// Standard print function, this one is set by default
        /**
         * @brief Default print function for Timer class.
         */
        static const std::string Simple(const std::string &title, [[maybe_unused]] std::size_t title_lenpadd, const ValueLabel &time) {
            return FORMAT(simpleFormat, title, time);
        }

        /**
         * @brief A more elaborate print function for Timer class.
         */
        static const std::string Big(const std::string &title, std::size_t title_lenpadd, const ValueLabel &time) {
            std::string times = FORMAT(bigTimesFormat, time);
            const auto times_len = times.length() + 3;
            const auto tot_len = title_lenpadd + times_len;
            const auto title_time_section = FORMAT(bigTitleTimeFormat, title, title_lenpadd - 4, times, times_len + 1);
            return FORMAT(bigFormat, "", tot_len, title_time_section);
        }

        /**
         * @brief A compact print function for Timer class.
         */
        static const std::string Compact(const std::string &title, [[maybe_unused]] std::size_t title_lenpadd, const ValueLabel &time) {
            return FORMAT(compactFormat, title, time);
        }

        /**
         * @brief A detailed print function for Timer class.
         */
        static const std::string Detailed(const std::string &title, [[maybe_unused]] std::size_t title_lenpadd, const ValueLabel &time) {
            return FORMAT(detailedFormat, title, time);
        }

        static std::string createPattern(std::size_t title_lenpadd) {
            const auto ntlenpadd = title_lenpadd / 4;
            return FORMAT(blockPatternFormat, "*", ntlenpadd);
        }
        /**
         * @brief A block style print function for Timer class.
         */
        static const std::string Block(const std::string &title, std::size_t title_lenpadd, const ValueLabel &time) {
            const auto patternf = createPattern(title_lenpadd);
            const auto times = FORMAT(blockTimesFormat, time);
            return FORMAT(blockFormat, patternf, title_lenpadd, title, times);
        }
        /**
         * @brief A minimal print function for Timer class.
         */
        static const std::string Minimal(const std::string &title, const std::string &time) { return FORMAT(minimalFormat, title, time); }

        /**
         * @brief Standard constructor for Timer class.
         *  Standard constructor, can set title and print function
         */
        explicit Timer(const std::string &title = "Timer", const time_print_t &time_print = Simple)
          : title_(title), title_lenpadd(title.length() + 10), time_print_(time_print), start_(clock::now()) {}

        Timer(const Timer &other) = delete;
        Timer &operator=(const Timer &other) = delete;
        Timer(Timer &&other) = delete;
        Timer &operator=(Timer &&other) = delete;

        [[nodiscard]] std::string time_it(const std::function<void()> &f, long double target_time = 1) {
            const time_point start = start_;
            auto total_time = std::numeric_limits<long double>::quiet_NaN();

            start_ = clock::now();
            std::size_t n = 0;
            do {
                f();
                nanolld elapsed = clock::now() - start_;
                total_time = elapsed.count();
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
        [[nodiscard]] inline long double make_time() const noexcept {
            const nanolld elapsed = clock::now() - start_;
            return elapsed.count();
        }

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
        [[nodiscard]] inline ValueLabel make_time_str() const noexcept {
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
        [[nodiscard]] static inline ValueLabel make_time_str(const long double time) noexcept {
            return make_named_times(time).getRelevantTimeframe();
        }
        // LCOV_EXCL_STOP

        /**
         * @brief Get a string representation of the Timer.
         * @return A string representation of the Timer.
         */
        [[nodiscard]] inline std::string to_string() const noexcept {
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

        AutoTimer(const AutoTimer &other) = delete;
        AutoTimer &operator=(const AutoTimer &other) = delete;
        AutoTimer(AutoTimer &&other) = delete;
        AutoTimer &operator=(AutoTimer &&other) = delete;

        ~AutoTimer() { LINFO(to_string()); }
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

inline std::ostream &operator<<(std::ostream &in, const vnd::Timer &timer) { return in << timer.to_string(); }

DISABLE_WARNINGS_POP()
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic pop
#endif

// NOLINTEND(*-include-cleaner, *-identifier-length, *-special-member-functions)
