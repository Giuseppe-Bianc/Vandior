/*
 * Created by gbian on 06/10/2024.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "VandiorCore/glm_prety_string_cast.hpp"

namespace glmp {
    namespace detail {
        template <typename T> constexpr char const *prefix<T>::value() { return TypePrefix<T>::value; }

        template <glm::qualifier Q> std::string compute_to_string<glm::vec<1, bool, Q>>::call(glm::vec<1, bool, Q> const &x) {
            return FORMAT("bvec1({})", x[0] ? detail::LabelTrue : detail::LabelFalse);
        }

        template <glm::qualifier Q> std::string compute_to_string<glm::vec<2, bool, Q>>::call(glm::vec<2, bool, Q> const &x) {
            return FORMAT("bvec2({}, {})", x[0] ? detail::LabelTrue : detail::LabelFalse, x[1] ? detail::LabelTrue : detail::LabelFalse);
        }

        template <glm::qualifier Q> std::string compute_to_string<glm::vec<3, bool, Q>>::call(glm::vec<3, bool, Q> const &x) {
            return FORMAT("bvec3({}, {}, {})", x[0] ? detail::LabelTrue : detail::LabelFalse, x[1] ? detail::LabelTrue : detail::LabelFalse,
                          x[2] ? detail::LabelTrue : detail::LabelFalse);
        }

        template <glm::qualifier Q> std::string compute_to_string<glm::vec<4, bool, Q>>::call(glm::vec<4, bool, Q> const &x) {
            return FORMAT("bvec4({}, {}, {}, {})", x[0] ? detail::LabelTrue : detail::LabelFalse,
                          x[1] ? detail::LabelTrue : detail::LabelFalse, x[2] ? detail::LabelTrue : detail::LabelFalse,
                          x[3] ? detail::LabelTrue : detail::LabelFalse);
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::vec<1, T, Q>>::call(glm::vec<1, T, Q> const &x) {
            return FORMAT("{}vec1({})", prefix<T>::value(), x[0]);
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::vec<2, T, Q>>::call(glm::vec<2, T, Q> const &x) {
            return FORMAT("{}vec2({}, {})", prefix<T>::value(), x[0], x[1]);
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::vec<3, T, Q>>::call(glm::vec<3, T, Q> const &x) {
            return FORMAT("{}vec3({}, {}, {})", prefix<T>::value(), x[0], x[1], x[2]);
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::vec<4, T, Q>>::call(glm::vec<4, T, Q> const &x) {
            return FORMAT("{}vec4({}, {}, {}, {})", prefix<T>::value(), x[0], x[1], x[2], x[3]);
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::mat<2, 2, T, Q>>::call(glm::mat<2, 2, T, Q> const &x) {
#ifdef PRETTY_PRINT
            return FORMAT("{}mat2x2(({},{}),\n ({},{}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1]);
#else
            return FORMAT("{}mat2x2(({},{}), ({},{}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1]);
#endif
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::mat<2, 3, T, Q>>::call(glm::mat<2, 3, T, Q> const &x) {
#ifdef PRETTY_PRINT
            return FORMAT("{}mat2x3(({}, {}, {}),\n ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[1][0], x[1][1],
                          x[1][2]);
#else
            return FORMAT("{}mat2x3(({}, {}, {}), ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[1][0], x[1][1], x[1][2]);
#endif
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::mat<2, 4, T, Q>>::call(glm::mat<2, 4, T, Q> const &x) {
#ifdef PRETTY_PRINT
            return FORMAT("{}mat2x4(({}, {}, {}, {}),\n ({}, {}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[0][3], x[1][0],
                          x[1][1], x[1][2], x[1][3]);
#else
            return FORMAT("{}mat2x4(({}, {}, {}, {}), ({}, {}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[0][3], x[1][0],
                          x[1][1], x[1][2], x[1][3]);
#endif
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::mat<3, 2, T, Q>>::call(glm::mat<3, 2, T, Q> const &x) {
#ifdef PRETTY_PRINT
            return FORMAT("{}mat3x2(({}, {}),\n ({}, {}),\n ({}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1], x[2][0],
                          x[2][1]);
#else
            return FORMAT("{}mat3x2(({}, {}), ({}, {}), ({}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1], x[2][0],
                          x[2][1]);
#endif
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::mat<3, 3, T, Q>>::call(glm::mat<3, 3, T, Q> const &x) {
#ifdef PRETTY_PRINT
            return FORMAT("{}mat3x3(({}, {}, {}),\n ({}, {}, {}),\n ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[1][0],
                          x[1][1], x[1][2], x[2][0], x[2][1], x[2][2]);
#else
            return FORMAT("{}mat3x3(({}, {}, {}), ({}, {}, {}), ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2], x[1][0],
                          x[1][1], x[1][2], x[2][0], x[2][1], x[2][2]);
#endif
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::mat<3, 4, T, Q>>::call(glm::mat<3, 4, T, Q> const &x) {
#ifdef PRETTY_PRINT
            return FORMAT("{}mat3x4(({}, {}, {}, {}),\n ({}, {}, {}, {}),\n ({}, {}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1],
                          x[0][2], x[0][3], x[1][0], x[1][1], x[1][2], x[1][3], x[2][0], x[2][1], x[2][2], x[2][3]);
#else
            return FORMAT("{}mat3x4(({}, {}, {}, {}), ({}, {}, {}, {}), ({}, {}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2],
                          x[0][3], x[1][0], x[1][1], x[1][2], x[1][3], x[2][0], x[2][1], x[2][2], x[2][3]);
#endif
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::mat<4, 2, T, Q>>::call(glm::mat<4, 2, T, Q> const &x) {
#ifdef PRETTY_PRINT
            return FORMAT("{}mat4x2(({}, {}),\n ({}, {}),\n ({}, {}),\n ({}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1],
                          x[2][0], x[2][1], x[3][0], x[3][1]);
#else
            return FORMAT("{}mat4x2(({}, {}), ({}, {}), ({}, {}), ({}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[1][0], x[1][1],
                          x[2][0], x[2][1], x[3][0], x[3][1]);
#endif
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::mat<4, 3, T, Q>>::call(glm::mat<4, 3, T, Q> const &x) {
#ifdef PRETTY_PRINT
            return FORMAT("{}mat4x3(({}, {}, {}),\n ({}, {}, {}),\n ({}, {}, {}),\n ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1],
                          x[0][2], x[1][0], x[1][1], x[1][2], x[2][0], x[2][1], x[2][2], x[3][0], x[3][1], x[3][2]);
#else
            return FORMAT("{}mat4x3(({}, {}, {}), ({}, {}, {}), ({}, {}, {}), ({}, {}, {}))", prefix<T>::value(), x[0][0], x[0][1], x[0][2],
                          x[1][0], x[1][1], x[1][2], x[2][0], x[2][1], x[2][2], x[3][0], x[3][1], x[3][2]);
#endif
        }

        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::mat<4, 4, T, Q>>::call(glm::mat<4, 4, T, Q> const &x) {
#ifdef PRETTY_PRINT
            return FORMAT("{}mat4x4(({}, {}, {}, {}),\n ({}, {}, {}, {}),\n ({}, {}, {}, {}),\n ({}, {}, {}, {}))", prefix<T>::value(),
                          x[0][0], x[0][1], x[0][2], x[0][3], x[1][0], x[1][1], x[1][2], x[1][3], x[2][0], x[2][1], x[2][2], x[2][3],
                          x[3][0], x[3][1], x[3][2], x[3][3]);
#else
            return FORMAT("{}mat4x4(({}, {}, {}, {}), ({}, {}, {}, {}), ({}, {}, {}, {}), ({}, {}, {}, {}))", prefix<T>::value(), x[0][0],
                          x[0][1], x[0][2], x[0][3], x[1][0], x[1][1], x[1][2], x[1][3], x[2][0], x[2][1], x[2][2], x[2][3], x[3][0],
                          x[3][1], x[3][2], x[3][3]);
#endif
        }

        template <typename T, glm::qualifier Q>
        std::string compute_to_string<glm::qua<T, Q>>::call(glm::qua<T, Q> const &q) {  // NOLINT(*-identifier-length)
            return FORMAT("{}quat({}, [{}, {}, {}])", prefix<T>::value(), q.w, q.x, q.y, q.z);
        }
        template <typename T, glm::qualifier Q> std::string compute_to_string<glm::tdualquat<T, Q>>::call(glm::tdualquat<T, Q> const &x) {
            return FORMAT("{}dualquat(({}, [{}, {}, {}]), ({}, [{}, {}, {}]))", prefix<T>::value(), x.real.w, x.real.x, x.real.y, x.real.z,
                          x.dual.w, x.dual.x, x.dual.y, x.dual.z);
        }
    }  // namespace detail

    template <class matType> std::string to_string(matType const &x) { return detail::compute_to_string<matType>::call(x); }

}  // namespace glmp

// NOLINTEND(*-include-cleaner)
