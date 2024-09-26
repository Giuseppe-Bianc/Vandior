#pragma once
#include "headers.hpp"

#define FORMAT_CONTAINER(container)                                                                                                        \
    std::string format = "{";                                                                                                              \
    for(const auto &i : container) { format = fmt::format("{}{}, ", format, i); }                                                          \
    if(format != "{") { format.erase(format.end() - 2, format.end()); }                                                                    \
    format += "}";                                                                                                                         \
    return fmt::format_to(ctx.out(), "{}", format);

namespace vnd {
    template <typename T, i64 N> class array : public std::array<T, N> {
    public:
        array() : std::array<T, N>(){};
        array(const std::vector<T> &vec) {
            if(vec.size() == N) {
                std::copy(vec.begin(), vec.end(), this->begin());
            } else {
                throw std::invalid_argument("Vector size does not match array size");
            }
        }
        array(std::initializer_list<T> init) : std::array<T, N>() {
            if(init.size() != N) { throw std::runtime_error("Initializer list size does not match array size."); }
            std::copy(init.begin(), init.end(), this->begin());
        }
        T &at(i64 index) {
            if(index < 0) { index += N; }
            if(index < 0 || index >= N) {
                throw std::runtime_error("Index " + std::to_string(index) + " out of bounds for size " + std::to_string(N));
            }
            return std::array<T, N>::at(index);
        }
        const T &at(i64 index) const {
            if(index < 0) { index += N; }
            if(index < 0 || index >= N) {
                throw std::runtime_error("Index " + std::to_string(index) + " out of bounds for size " + std::to_string(N));
            }
            return std::array<T, N>::at(index);
        }
    };
    template <typename T> class vector : public std::vector<T> {
    public:
        vector() : std::vector<T>(){};
        vector(std::initializer_list<T> init) : std::vector<T>(init) {}
        template <typename InputIt> vector(InputIt first, InputIt last) : std::vector<T>(first, last) {}
        T &at(i64 index) {
            if(index < 0) { index += static_cast<i64>(this->size()); }
            if(index < 0 || index >= static_cast<i64>(this->size())) {
                throw std::runtime_error("Index " + std::to_string(index) + " out of bounds for size " +
                                         std::to_string(static_cast<int64_t>(this->size())));
            }
            return std::vector<T>::at(index);
        }
        const T at(i64 index) const {
            if(index < 0) { index += static_cast<i64>(this->size()); }
            if(index < 0 || index >= static_cast<i64>(this->size())) {
                throw std::runtime_error("Index " + std::to_string(index) + " out of bounds for size " +
                                         std::to_string(static_cast<int64_t>(this->size())));
            }
            return std::vector<T>::at(index);
        }
        void addVector(const vnd::vector<T> elems) {
            for(const T &i : elems) { std::vector<T>::emplace_back(i); }
        }
        void addAll(const vnd::vector<T> elems) {
            for(const T &i : elems) { std::vector<T>::emplace_back(i); }
        }
        void add(const T elem) { std::vector<T>::emplace_back(elem); }
    };
}  // namespace vnd

/**
 * This function is a formatter for Token using fmt.
 * \cond
 */
template <typename T, i64 N> struct fmt::formatter<vnd::array<T, N>> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
    template <typename FormatContext> auto format(const vnd::array<T, N> &arr, FormatContext &ctx) { FORMAT_CONTAINER(arr); }
};
template <typename T> struct fmt::formatter<vnd::vector<T>> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
    template <typename FormatContext> auto format(const vnd::vector<T> &vec, FormatContext &ctx) { FORMAT_CONTAINER(vec); }
};
/** \endcond */