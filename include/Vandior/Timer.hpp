#pragma once
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
#include "Log.hpp"
#include "disableWarn.hpp"
#include "format.hpp"
#include "headers.hpp"

// On GCC < 4.8, the following define is often missing. Since
// this library only uses sleep_for, this should be safe
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5 && __GNUC_MINOR__ < 8
#define _GLIBCXX_USE_NANOSLEEP
#endif

namespace vnd {

    /**
     * @brief A factor for converting microseconds to seconds.
     *
     * This constant represents the conversion factor from microseconds to seconds.
     * It is used to convert time values from microseconds to seconds by multiplying
     * the time value with this factor.
     */
    inline static constexpr long double MICROSECONDSFACTOR = 1000.0L;

    /**
     * @brief A factor for converting milliseconds to seconds.
     *
     * This constant represents the conversion factor from milliseconds to seconds.
     * It is used to convert time values from milliseconds to seconds by multiplying
     * the time value with this factor.
     */
    inline static constexpr long double MILLISECONDSFACTOR = 1'000'000.0L;

    /**
     * @brief A factor for converting seconds to seconds (1 billion nanoseconds).
     *
     * This constant represents the conversion factor from seconds to seconds, specifically
     * in the unit of nanoseconds. It is used to convert time values from seconds to
     * nanoseconds by multiplying the time value with this factor.
     */
    inline static constexpr long double SECONDSFACTOR = 1'000'000'000.0L;

    /**
     * @brief A multiplier factor used in the Timer class.
     *
     * This constant represents a multiplier factor used internally in the Timer class
     * for certain calculations or operations related to time. Its specific purpose
     * may vary depending on the implementation within the Timer class.
     */
    inline static constexpr long MFACTOR = 100;

    DISABLE_WARNINGS_PUSH(6005 26447 26455 26496)

    // OLINTBEGIN(*-include-cleaner)
    /**
     * @brief Timer class for measuring the execution time of code.
     */
    class Timer {  // NOLINT(*-special-member-functions)
    protected:
        /// This is a typedef to make clocks easier to use
        using clock = std::chrono::high_resolution_clock;  // NOLINT(*-include-cleaner)
        using times = std::tuple<long double, long double, long double, long double, std::string, std::string, std::string, std::string>;

        /// This typedef is for points in time
        using time_point = std::chrono::time_point<clock>;  // NOLINT(*-include-cleaner)

        /// This is the type of a printing function, you can make your own
        using time_print_t = std::function<std::string(std::string, std::string)>;  // NOLINT(*-include-cleaner)
        using nanolld = std::chrono::duration<long double, std::nano>;
        /// This is the title of the timer
        std::string title_;

        /// This is the function that is used to format most of the timing message
        time_print_t time_print_;

        /// This is the starting point (when the timer was created)
        time_point start_;

        /// This is the number of times cycles (print divides by this number)
        std::size_t cycles{1};

    public:
        /// Standard print function, this one is set by default
        // NOLINTBEGIN
        /**
         * @brief Default print function for Timer class.
         */
        static const std::string Simple(const std::string &title, const std::string &time) { return FORMAT("{}: {}", title, time); }

        /**
         * @brief A more elaborate print function for Timer class.
         */
        static const std::string Big(const std::string &title, const std::string &time) {
            return FORMAT("{0:-^{1}}\n| {2} | Time = {3}\n{0:-^{1}}", "", 41, title, time);
        }
        // NOLINTEND

        /**
         * @brief Standard constructor for Timer class.
         *  Standard constructor, can set title and print function
         */
        explicit Timer(const std::string &title = "Timer", const time_print_t &time_print = Simple)
          : title_(title), time_print_(time_print), start_(clock::now()) {}

        Timer(const Timer &other) = delete;              /// Delete copy constructor
        Timer &operator=(const Timer &other) = delete;   /// Delete copy assignment operator
        Timer(const Timer &&other) = delete;             /// Delete move constructor
        Timer &operator=(const Timer &&other) = delete;  /// Delete move assignment operator

        /**
         * @brief Time a function by running it multiple times.
         * @param f The function to be timed.
         * @param target_time Target time in seconds.
         * @return A string with timing information.
         */
        // NOLINTNEXTLINE(*-identifier-length)
        [[nodiscard]] std::string time_it(const std::function<void()> &f, long double target_time = 1) {
            const time_point start = start_;
            // NOLINTNEXTLINE(clang-analyzer-cplusplus.InnerPointer)
            auto total_time = C_LD(NAN);

            start_ = clock::now();
            std::size_t n = 0;  // NOLINT(*-identifier-length)
            do {                // NOLINT(*-avoid-do-while)
                f();
                nanolld elapsed = clock::now() - start_;
                total_time = elapsed.count();
            } while(n++ < MFACTOR && total_time < target_time);

            std::string out = FORMAT("{} for {} tries", make_time_str(C_LD(total_time / C_LD(n))), std::to_string(n));
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

        // NOLINTBEGIN
        /**
         * @brief Get the named times (seconds, milliseconds, microseconds, nanoseconds).
         * @param time The time in nanoseconds.
         * @return A tuple containing named times.
         */
        [[nodiscard]] static times make_named_times(long double time) {  // NOLINT(*-identifier-length)
            const auto &secondsTime = time / SECONDSFACTOR;
            const auto &millisTime = time / MILLISECONDSFACTOR;
            const auto &microTime = time / MICROSECONDSFACTOR;
            return {secondsTime, millisTime, microTime, time, "s", "ms", "us", "ns"};
        }
        // NOLINTEND

        [[maybe_unused]] [[nodiscard]] times multi_time() const { return make_named_times(make_time()); }

        /**
         * @brief Get the named time and its unit.
         * @param time The time in nanoseconds.
         * @return A pair containing the named time and its unit.
         */
        [[nodiscard]] static std::pair<long double, std::string> make_named_time(long double time) {
            const auto &[ld1, ld2, ld3, ld4, str1, str2, str3, str4] = make_named_times(time);
            // Accessing values
            if(ld1 > 1) [[likely]] {  // seconds
                return {ld1, str1};
            } else if(ld2 > 1) [[likely]] {  // milli
                return {ld2, str2};
            } else if(ld3 > 1) [[likely]] {  // micro
                return {ld3, str3};
            } else [[unlikely]] {
                return {ld4, str4};
            }
        }

        /**
         * @brief Format the numerical value for the time string.
         * This formats the numerical value for the time string
         * @return A formatted time string.
         */
        [[nodiscard]] inline std::string make_time_str() const {  // NOLINT(modernize-use-nodiscard)
            const auto time = make_time() / C_LD(cycles);
            return make_time_str(time);
        }

        // LCOV_EXCL_START
        /**
         * @brief Format a given time value into a string.
         * This prints out a time string from a time
         * @param time The time value in nanoseconds.
         * @return A formatted time string.
         */
        [[nodiscard]] static inline std::string make_time_str(long double time) {  // NOLINT(modernize-use-nodiscard)
            const auto &[titme, stime] = make_named_time(time);
            return FORMAT("{:.f} {}", titme, stime);
        }
        // LCOV_EXCL_STOP

        /**
         * @brief Get a string representation of the Timer.
         * @return A string representation of the Timer.
         */
        [[nodiscard]] inline std::string to_string() const noexcept { return std::invoke(time_print_, title_, make_time_str()); }

        /**
         * @brief Set the number of cycles to divide by.
         * Division sets the number of cycles to divide by (no graphical change)
         * @param val Number of cycles.
         * @return A reference to the Timer object.
         */
        Timer &operator/(std::size_t val) noexcept {
            cycles = val;
            return *this;
        }
    };

    /**
     * @brief Automatic Timer class that prints out the time upon destruction.
     */
    class AutoTimer : public Timer {
    public:
        using Timer::Timer;

        // Delete copy and move operations in AutoTimer
        AutoTimer(const AutoTimer &other) = delete;
        AutoTimer &operator=(const AutoTimer &other) = delete;
        AutoTimer(AutoTimer &&other) = delete;
        AutoTimer &operator=(AutoTimer &&other) = delete;

        /**
         * @brief Destructor for AutoTimer class that prints the time string.
         */
        ~AutoTimer() { LINFO(to_string()); }
    };
}  // namespace vnd

/** \cond */
/**
 * @brief Specialization of the fmt::formatter for the Timer class.
 */
template <> struct fmt::formatter<vnd::Timer> : formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    /**
     * @brief Format the Timer object into a string view.
     * @param timer The Timer object.
     * @param ctx The format context.
     * @return A formatted string view.
     */
    template <typename FormatContext> auto format(const vnd::Timer &timer, FormatContext &ctx) {
        return formatter<std::string_view>::format(timer.to_string(), ctx);
    }
};
/** \endcond */

/// This prints out the time if shifted into a std::cout like stream.
inline std::ostream &operator<<(std::ostream &in, const vnd::Timer &timer) { return in << timer.to_string(); }

// OLINTEND
DISABLE_WARNINGS_POP()
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic pop
#endif
