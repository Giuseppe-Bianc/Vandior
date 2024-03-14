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
            /**
             * @brief Get the type prefix for the generic type.
             * @return The type prefix string.
             */
            static constexpr char const *value() { return TypePrefix<T>::value; }
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
            /**
             * @brief Converts glm::vec<1, bool, Q> to a string.
             * @param x The input vector.
             * @return String representation of the vector.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<1, bool, Q> const &x) {
                return FORMAT("bvec1({})", x[0] ? detail::LabelTrue : detail::LabelFalse);
            }
        };
        /**
         * @brief Specialization for glm::vec<2, bool, Q>.
         */
        template <glm::qualifier Q> struct compute_to_string<glm::vec<2, bool, Q>> {
            /**
             * @brief Converts glm::vec<2, bool, Q> to a string.
             * @param x The input vector.
             * @return String representation of the vector.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<2, bool, Q> const &x) {
                return FORMAT("bvec2({}, {})", x[0] ? detail::LabelTrue : detail::LabelFalse,
                              x[1] ? detail::LabelTrue : detail::LabelFalse);
            }
        };
        /**
         * @brief Specialization for glm::vec<3, bool, Q>.
         */
        template <glm::qualifier Q> struct compute_to_string<glm::vec<3, bool, Q>> {
            /**
             * @brief Converts glm::vec<3, bool, Q> to a string.
             * @param x The input vector.
             * @return String representation of the vector.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<3, bool, Q> const &x) {
                return FORMAT("bvec3({}, {}, {})", x[0] ? detail::LabelTrue : detail::LabelFalse,
                              x[1] ? detail::LabelTrue : detail::LabelFalse, x[2] ? detail::LabelTrue : detail::LabelFalse);
            }
        };

        /**
         * @brief Specialization for glm::vec<4, bool, Q>.
         */
        template <glm::qualifier Q> struct compute_to_string<glm::vec<4, bool, Q>> {
            /**
             * @brief Converts glm::vec<4, bool, Q> to a string.
             * @param x The input vector.
             * @return String representation of the vector.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<4, bool, Q> const &x) {
                return FORMAT("bvec4({}, {}, {}, {})", x[0] ? detail::LabelTrue : detail::LabelFalse,
                              x[1] ? detail::LabelTrue : detail::LabelFalse, x[2] ? detail::LabelTrue : detail::LabelFalse,
                              x[3] ? detail::LabelTrue : detail::LabelFalse);
            }
        };

        /**
         * @brief Provides functions to convert glm::vec types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::vec<1, T, Q>> {
            /**
             * @brief Convert glm::vec1 to string.
             * @param x The glm::vec1 to be converted.
             * @return The string representation of glm::vec1.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<1, T, Q> const &x) {
                return FORMAT("{}vec1({})", prefix<T>::value(), x[0]);
            }
        };

        /**
         * @brief Provides functions to convert glm::vec types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::vec<2, T, Q>> {
            /**
             * @brief Convert glm::vec2 to string.
             * @param x The glm::vec2 to be converted.
             * @return The string representation of glm::vec2.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<2, T, Q> const &x) {
                return FORMAT("{}vec2({}, {})", prefix<T>::value(), x[0], x[1]);
            }
        };

        /**
         * @brief Provides functions to convert glm::vec types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::vec<3, T, Q>> {
            /**
             * @brief Convert glm::vec3 to string.
             * @param x The glm::vec3 to be converted.
             * @return The string representation of glm::vec3.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<3, T, Q> const &x) {
                return FORMAT("{}vec3({}, {}, {})", prefix<T>::value(), x[0], x[1], x[2]);
            }
        };

        /**
         * @brief Provides functions to convert glm::vec types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::vec<4, T, Q>> {
            /**
             * @brief Convert glm::vec4 to string.
             * @param x The glm::vec4 to be converted.
             * @return The string representation of glm::vec4.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::vec<4, T, Q> const &x) {
                return FORMAT("{}vec4({}, {}, {}, {})", prefix<T>::value(), x[0], x[1], x[2], x[3]);
            }
        };
        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<2, 2, T, Q>> {
            /**
             * @brief Convert glm::mat2x2 to string.
             * @param x The  glm::mat2x2 to be converted.
             * @return The string representation of  glm::mat2x2.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<2, 2, T, Q> const &x) {
#ifdef PRETTY_PRINT
                return FORMAT("{}mat2x2(({},{}),\n ({},{}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1]);
#else
                return FORMAT("{}mat2x2(({},{}), ({},{}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1]);
#endif
            }
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<2, 3, T, Q>> {
            /**
             * @brief Convert glm::mat2x3 to string.
             * @param x The  glm::mat2x3 to be converted.
             * @return The string representation of  glm::mat2x3.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<2, 3, T, Q> const &x) {
#ifdef PRETTY_PRINT
                return FORMAT("{}mat2x3(({}, {}, {}),\n ({}, {}, {}))", x[0][0], x[0][1], x[0][2], x[1][0], x[1][1], x[1][2]);
#else
                return FORMAT("{}mat2x3(({}, {}, {}), ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[1][0], x[1][1],
                              x[1][2]);
#endif
            }
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<2, 4, T, Q>> {
            /**
             * @brief Convert glm::mat2x4 to string.
             * @param x The  glm::mat2x4 to be converted.
             * @return The string representation of  glm::mat2x4.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<2, 4, T, Q> const &x) {
#ifdef PRETTY_PRINT
                return FORMAT("{}mat2x4(({}, {}, {}, {}),\n ({}, {}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[0][3],
                              x[1][0], x[1][1], x[1][2], x[1][3]);
#else
                return FORMAT("{}mat2x4(({}, {}, {}, {}), ({}, {}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[0][3],
                              x[1][0], x[1][1], x[1][2], x[1][3]);
#endif
            }
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<3, 2, T, Q>> {
            /**
             * @brief Convert glm::mat3x2 to string.
             * @param x The  glm::mat3x2 to be converted.
             * @return The string representation of  glm::ma3x2.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<3, 2, T, Q> const &x) {
#ifdef PRETTY_PRINT
                return FORMAT("{}mat3x2(({}, {}),\n ({}, {}),\n ({}, {}))" prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1], x[2][0],
                              x[2][1]);
#else
                return FORMAT("{}mat3x2(({}, {}), ({}, {}), ({}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1], x[2][0],
                              x[2][1]);
#endif
            }
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<3, 3, T, Q>> {
            /**
             * @brief Convert glm::mat3x3 to string.
             * @param x The  glm::mat3x3 to be converted.
             * @return The string representation of  glm::mat3x3.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<3, 3, T, Q> const &x) {
#ifdef PRETTY_PRINT
                return FORMAT("{}mat3x3(({}, {}, {}),\n ({}, {}, {}),\n ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2],
                              x[1][0], x[1][1], x[1][2], x[2][0], x[2][1], x[2][2]);
#else
                return FORMAT("{}mat3x3(({}, {}, {}), ({}, {}, {}), ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[1][0],
                              x[1][1], x[1][2], x[2][0], x[2][1], x[2][2]);
#endif
            }
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<3, 4, T, Q>> {
            /**
             * @brief Convert glm::mat3x4 to string.
             * @param x The glm::mat3x4 to be converted.
             * @return The string representation of glm::mat3x4.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<3, 4, T, Q> const &x) {
#ifdef PRETTY_PRINT
                return FORMAT("{}mat3x4(({}, {}, {}, {}),\n ({}, {}, {}, {}),\n ({}, {}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1],
                              x[0][2], x[0][3], x[1][0], x[1][1], x[1][2], x[1][3], x[2][0], x[2][1], x[2][2], x[2][3]);
#else
                return FORMAT("{}mat3x4(({}, {}, {}, {}), ({}, {}, {}, {}), ({}, {}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1],
                              x[0][2], x[0][3], x[1][0], x[1][1], x[1][2], x[1][3], x[2][0], x[2][1], x[2][2], x[2][3]);
#endif
            }
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<4, 2, T, Q>> {
            /**
             * @brief Convert glm::mat4x2 to string.
             * @param x The  glm::mat4x2 to be converted.
             * @return The string representation of  glm::mat4x2.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<4, 2, T, Q> const &x) {
#ifdef PRETTY_PRINT
                return FORMAT("{}mat4x2(({}, {}),\n ({}, {}),\n ({}, {}),\n ({}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0],
                              x[1][1], x[2][0], x[2][1], x[3][0], x[3][1]);
#else
                return FORMAT("{}mat4x2(({}, {}), ({}, {}), ({}, {}), ({}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1],
                              x[2][0], x[2][1], x[3][0], x[3][1]);
#endif
            }
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<4, 3, T, Q>> {
            /**
             * @brief Convert glm::mat4x2 to string.
             * @param x The  glm::mat4x2 to be converted.
             * @return The string representation of  glm::mat4x2.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<4, 3, T, Q> const &x) {
#ifdef PRETTY_PRINT
                return FORMAT("{}mat4x3(({}, {}, {}),\n ({}, {}, {}),\n ({}, {}, {}),\n ({}, {}, {}))", prefix<T>::value(), x[0][0],
                              x[0][1], x[0][2], x[1][0], x[1][1], x[1][2], x[2][0], x[2][1], x[2][2], x[3][0], x[3][1], x[3][2]);
#else
                return FORMAT("{}mat4x3(({}, {}, {}), ({}, {}, {}), ({}, {}, {}), ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1],
                              x[0][2], x[1][0], x[1][1], x[1][2], x[2][0], x[2][1], x[2][2], x[3][0], x[3][1], x[3][2]);
#endif
            }
        };

        /**
         * @brief Provides functions to convert glm::mat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::mat<4, 4, T, Q>> {
            /**
             * @brief Convert glm::mat4x4 to string.
             * @param x The glm::mat4x4 to be converted.
             * @return The string representation of  glm::mat4x4.
             */
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::mat<4, 4, T, Q> const &x) {
#ifdef PRETTY_PRINT
                return FORMAT("{}mat4x4(({}, {}, {}, {}),\n ({}, {}, {}, {}),\n ({}, {}, {}, {}),\n ({}, {}, {}, {}))", prefix<T>::value(),
                              x[0][0], x[0][1], x[0][2], x[0][3], x[1][0], x[1][1], x[1][2], x[1][3], x[2][0], x[2][1], x[2][2], x[2][3],
                              x[3][0], x[3][1], x[3][2], x[3][3]);
#else
                return FORMAT("{}mat4x4(({}, {}, {}, {}), ({}, {}, {}, {}), ({}, {}, {}, {}), ({}, {}, {}, {}))", prefix<T>::value(),
                              x[0][0], x[0][1], x[0][2], x[0][3], x[1][0], x[1][1], x[1][2], x[1][3], x[2][0], x[2][1], x[2][2], x[2][3],
                              x[3][0], x[3][1], x[3][2], x[3][3]);
#endif
            }
        };

        /**
         * @brief Provides functions to convert glm::qua types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::qua<T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::qua<T, Q> const &q) {  // NOLINT(*-identifier-length)
                return FORMAT("{}quat({}, [{}, {}, {}])", prefix<T>::value(), q.w, q.x, q.y, q.z);
            }
        };

        /**
         * @brief Provides functions to convert glm::tdualquat types to strings.
         */
        template <typename T, glm::qualifier Q> struct compute_to_string<glm::tdualquat<T, Q>> {
            [[nodiscard]] GLMP_FUN_QUAL static std::string call(glm::tdualquat<T, Q> const &x) {
                return FORMAT("{}dualquat(({}, [{}, {}, {}]), ({}, [{}, {}, {}]))", prefix<T>::value(), x.real.w, x.real.x, x.real.y,
                              x.real.z, x.dual.w, x.dual.x, x.dual.y, x.dual.z);
            }
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
    template <class matType> [[nodiscard]] GLMP_FUN_QUAL std::string to_string(matType const &x) {
        return detail::compute_to_string<matType>::call(x);
    }

}  // namespace glmp
