#pragma once

#include "format.hpp"     // NOLINT(*-include-cleaner)
#include "glm_matld.hpp"  // NOLINT(*-include-cleaner)

// #define PRETTY_PRINT

#define GLMP_FUN_QUAL GLM_FUNC_QUALIFIER
namespace glmp {
    namespace detail {

        /**
         * @brief String label for the boolean value 'true'.
         */
        static inline constexpr const char *LabelTrue = "true";

        /**
         * @brief String label for the boolean value 'false'.
         */
        static inline constexpr const char *LabelFalse = "false";

        /**
         * @brief Template struct to provide type prefixes for generic types.
         * @tparam T The generic data type.
         */
        template <typename T> struct TypePrefix {
            /**
             * @brief Type prefix value for the generic type.
             */
            static constexpr char const *value = "";
        };

        /**
         * @brief Specialization for the float type.
         */
        template <> struct TypePrefix<float> {
            /**
             * @brief Type prefix value for the float type.
             */
            static constexpr char const *value = "";
        };

        /**
         * @brief Specialization for the double type.
         */
        template <> struct TypePrefix<double> {
            /**
             * @brief Type prefix value for the double type.
             */
            static constexpr char const *value = "d";
        };

        /**
         * @brief Specialization for the long double type.
         */
        template <> struct TypePrefix<long double> {
            /**
             * @brief Type prefix value for the long double type.
             */
            static constexpr char const *value = "ld";
        };

        /**
         * @brief Specialization for the bool type.
         */
        template <> struct TypePrefix<bool> {
            /**
             * @brief Type prefix value for the bool type.
             */
            static constexpr char const *value = "b";
        };

        /**
         * @brief Specialization for the uint8_t type.
         */
        template <> struct TypePrefix<uint8_t> {
            /**
             * @brief Type prefix value for the uint8_t type.
             */
            static constexpr char const *value = "u8";
        };

        /**
         * @brief Specialization for the int8_t type.
         */
        template <> struct TypePrefix<int8_t> {
            /**
             * @brief Type prefix value for the int8_t type.
             */
            static constexpr char const *value = "i8";
        };

        // Repeat the pattern for other data types...

        /**
         * @brief Template struct to provide a function for obtaining type prefixes for generic types.
         * @tparam T The generic data type.
         */
        template <typename T> struct prefix {
            static constexpr char const *value();
        };

        /**
         * @brief Template structure for converting glm vector types to strings.
         * @tparam matType The glm vector type.
         */
        template <typename matType> struct compute_to_string {};

        /**
         * @brief Specialization for glm::vec<1, bool, Q>.
         */
        template <glm::qualifier Q> struct compute_to_string<glm::vec<1, bool, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<1, bool, Q> const &x);
        };
        /**
         * @brief Specialization for glm::vec<2, bool, Q>.
         */
        template <glm::qualifier Q> struct compute_to_string<glm::vec<2, bool, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<2, bool, Q> const &x);
        };

        /**
         * @brief Specialization for glm::vec<3, bool, Q>.
         */
        template <glm::qualifier Q> struct compute_to_string<glm::vec<3, bool, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<3, bool, Q> const &x);
        };

        /**
         * @brief Specialization for glm::vec<4, bool, Q>.
         */
        template <glm::qualifier Q> struct compute_to_string<glm::vec<4, bool, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<4, bool, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::vec types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::vec<1, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<1, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::vec types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::vec<2, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<2, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::vec types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::vec<3, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<3, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::vec types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::vec<4, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<4, T, Q> const &x);
        };

        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<2, 2, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<2, 2, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<2, 3, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<2, 3, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<2, 4, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<2, 4, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<3, 2, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<3, 2, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<3, 3, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<3, 3, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<3, 4, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<3, 4, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<4, 2, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<4, 2, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<4, 3, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<4, 3, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<4, 4, T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<4, 4, T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::qua types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::qua<T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::qua<T, Q> const &x);
        };

        /**
         * @brief Provides functions to convert glm::tdualquat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::tdualquat<T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::tdualquat<T, Q> const &x);
        };
    }  // namespace detail

    /**
     * @brief Converts a generic object into a string.
     * This function uses the compute_to_string class to convert the input object into a string.
     * The compute_to_string class must be specialized for the matType type.
     * @tparam matType The type of the object to be converted into a string.
     * @param x The object to be converted into a string.
     * @return A string that represents the object x.
     * @note This function is marked as [[nodiscard]] to indicate that the compiler should emit a warning if the return value is
     * not used.
     */
    template <class matType> [[nodiscard]] GLMP_FUN_QUAL std::string to_string(matType const &x);

}  // namespace glmp