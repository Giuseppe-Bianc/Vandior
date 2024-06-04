#pragma once
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
#include "../Log.hpp"
#include "../disableWarn.hpp"
#include "../format.hpp"
#include "TimePoints.hpp"
#include "Times.hpp"
#include "cronoRedefinition.hpp"
// On GCC < 4.8, the following define is often missing. Since
// this library only uses sleep_for, this should be safe
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5 && __GNUC_MINOR__ < 8
#define _GLIBCXX_USE_NANOSLEEP
#endif

namespace vnd {
    DISABLE_WARNINGS_PUSH(6005 26447 26455 26496)

    // NOLINTBEGIN(*-include-cleaner)
    /**
     * @brief Timer class for measuring the execution time of code.
     */
    class Timer {  // NOLINT(*-special-member-functions)
    protected:
        /// This is the type of a printing function, you can make your own
        using time_print_t = std::function<std::string(std::string, std::string)>;
        /// This is the title of the timer
        std::string title_;

        /// This is the function that is used to format most of the timing message
        time_print_t time_print_;

        /// This is the starting point (when the timer was created)
        TimePoints start_;

        /// This is the number of times cycles (print divides by this number)
        std::size_t cycles{1};

    public:
        /// Standard print function, this one is set by default
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

        /**
         * @brief Standard constructor for Timer class.
         *  Standard constructor, can set title and print function
         */
        explicit Timer(const std::string &title = "Timer", const time_print_t &time_print = Simple)
          : title_(title), time_print_(time_print),
            start_(clock::now(), utc_clock::now(), tai_clock::now(), gps_clock::now(), file_clock::now()) {}

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
            const TimePoints start = start_;
            // NOLINTNEXTLINE(clang-analyzer-cplusplus.InnerPointer)
            auto total_times = Clocks{C_LD(NAN), C_LD(NAN), C_LD(NAN), C_LD(NAN), C_LD(NAN)};
            start_ = {clock::now(), utc_clock::now(), tai_clock::now(), gps_clock::now(), file_clock::now()};
            std::size_t n = 0;  // NOLINT(*-identifier-length)
            do {
                // NOLINT(*-avoid-do-while)
                f();
                auto elapsed = TimePoints{clock::now(), utc_clock::now(), tai_clock::now(), gps_clock::now(), file_clock::now()} - start_;
                total_times = elapsed.toClocks();
            } while(n++ < vnd::MFACTOR && total_times.time < target_time);
            const auto ldn = C_LD(n);
            const auto mktstr = make_time_str({total_times.time / ldn, total_times.utc_time / ldn, total_times.tai_time / ldn,
                                               total_times.gps_time / ldn, total_times.file_time / ldn});
            std::string out = FORMAT("{} for {} tries", mktstr, std::to_string(n));
            start_ = start;
            return out;
        }

        /**
         * @brief Get the elapsed time in seconds.
         * @return Elapsed time in seconds.
         */
        [[nodiscard]] inline Clocks make_time() const noexcept {
            const auto elapsed = TimePoints{clock::now(), utc_clock::now(), tai_clock::now(), gps_clock::now(), file_clock::now()} - start_;
            return elapsed.toClocks();
        }
        /**
         * @brief Get the named times (seconds, milliseconds, microseconds, nanoseconds).
         * @param times The time in nanoseconds.
         * @return A tuple containing named times.
         */
        [[nodiscard]] static Times make_named_times(Clocks times) {  // NOLINT(*-identifier-length)
            return {times.getInSeconds(), times.getinMicros(), times.getinMicros(), times, "s", "ms", "us", "ns"};
        }

        [[maybe_unused]] [[nodiscard]] Times multi_time() const { return make_named_times(make_time()); }

        /**
         * @brief Get the named time and its unit.
         * @param intime The time in nanoseconds.
         * @return A pair containing the named time and its unit.
         */
        [[nodiscard]] static std::pair<Clocks, std::string> make_named_time(const Clocks &intime) {
            const auto &times = make_named_times(intime);
            const auto &[ld1, utc_ld1, tai_ld1, gps_ld1, file_ld1] = times.tseconds;
            const auto &[ld2, utc_ld2, tai_ld2, gps_ld2, file_ld2] = times.tmillis;
            const auto &[ld3, utc_ld3, tai_ld3, gps_ld3, file_ld3] = times.tmicros;
            const auto &[ld4, utc_ld4, tai_ld4, gps_ld4, file_ld4] = times.tnanos;
            //  Accessing values
            if(ld1 > 1 && utc_ld1 > 1 && tai_ld1 > 1 && gps_ld1 > 1 && file_ld1 > 1) [[likely]] {  // seconds
                return {{ld1, utc_ld1, tai_ld1, gps_ld1, file_ld1}, times.seconds};
            } else if(ld2 > 1 && utc_ld2 > 1 && tai_ld2 > 1 && gps_ld2 > 1 && file_ld2 > 1) [[likely]] {  // milli
                return {{ld2, utc_ld2, tai_ld2, gps_ld2, file_ld2}, times.millis};
            } else if(ld3 > 1 && utc_ld3 > 1 && tai_ld3 > 1 && gps_ld3 > 1 && file_ld3 > 1) [[likely]] {  // micro
                return {{ld3, utc_ld3, tai_ld3, gps_ld3, file_ld3}, times.micros};
            } else [[unlikely]] {
                return {{ld4, utc_ld4, tai_ld4, gps_ld4, file_ld4}, times.nanos};
            }
        }

        /**
         * @brief Format the numerical value for the time string.
         * This formats the numerical value for the time string
         * @return A formatted time string.
         */
        [[nodiscard]] inline std::string make_time_str() const {  // NOLINT(modernize-use-nodiscard)
            const auto &[time, utc_time, tai_time, gps_time, file_time] = make_time();
            const auto ldcycles = C_LD(cycles);
            return make_time_str({time / ldcycles, utc_time / ldcycles, tai_time / ldcycles, gps_time / ldcycles, file_time / ldcycles});
        }

        // LCOV_EXCL_START
        /**
         * @brief Format a given time value into a string.
         * This prints out a time string from a time
         * @param intime The time value in nanoseconds.
         * @return A formatted time string.
         */
        [[nodiscard]] static inline std::string make_time_str(const Clocks &intime) {  // NOLINT(modernize-use-nodiscard)
            const auto &[titmes, stime] = make_named_time(intime);
            const auto &[time, utc_time, tai_time, gps_time, file_time] = titmes;

            const auto stimes = FORMAT("Sys: {:.3Lf} {}", time, stime);
            const auto sutc_time = FORMAT("UTC: {:.3Lf} {}", utc_time, stime);
            const auto stai_time = FORMAT("TAI: {:.3Lf} {}", tai_time, stime);
            const auto sgps_time = FORMAT("GPS: {:.3Lf} {}", gps_time, stime);
            const auto sfile_time = FORMAT("File: {:.3Lf} {}", file_time, stime);
            return FORMAT("{},{},{},{},{}", stimes, sutc_time, stai_time, sgps_time, sfile_time);
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
    };  // namespace vnd

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

// NOLINTEND(*-include-cleaner)
DISABLE_WARNINGS_POP()
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic pop
#endif
