#pragma once
#include "primitives.hpp"

class string {
public:
    string(const std::string &s = "") : str(string::create(s)) {}
    string(const string &s) : str(string::create(*(s.str))) {}
    ~string() { string::deleteString(str); }
    const i64 size() const { return str->size(); }
    const char at(i64 index) const {
        if(index < 0) { index += static_cast<i64>(size()); }
        if(index < 0 || index >= size()) {
            throw std::runtime_error("Index " + std::to_string(index) + " out of bounds for size " + std::to_string(size()));
        }
        return str->at(index);
    }
    string lower() {
        std::string result;
        for(const char c : *str) { result += std::tolower(c); }
        return string(result);
    }
    string upper() {
        std::string result;
        for(const char c : *str) { result += std::toupper(c); }
        return string(result);
    }
    const bool empty() { return str->empty(); }
    const i32 toI32() { return static_cast<i32>(std::stoi(*str)); }
    const f32 toF32() { return static_cast<f32>(std::stof(*str)); }
    const c32 toC32() {
        f32 real{}, imag{};
        char sign = '+';
        std::istringstream iss(*str);
        size_t pos_i = str->find('i');
        if(pos_i != std::string::npos) {
            iss >> imag;
            if(pos_i > 0) {
                iss >> sign;
                if(sign == '-') imag *= -1;
                if(iss.peek() == 'i') iss.ignore();
            }
        }
        if(iss >> real) {
            if(sign == '-') imag *= -1;
        }
        return c32(real, imag);
    }
    std::string::const_iterator begin() const noexcept { return str->begin(); }
    std::string::const_iterator end() const noexcept { return str->end(); }
    string operator=(const string &s) {
        if(this != &s) {
            std::shared_ptr<const std::string> old = str;
            str = string::create(*(s.str));
            string::deleteString(old);
        }
        return *this;
    }
    friend bool operator==(const string &s1, const string &s2) { return *s1.str == *s2.str; }
    friend std::ostream &operator<<(std::ostream &os, const string &s) {
        os << *(s.str);
        return os;
    }
    operator std::string() const { return *str; }

private:
    std::shared_ptr<const std::string> str;
    static std::vector<std::pair<std::shared_ptr<const std::string>, i64>> pool;
    static std::shared_ptr<const std::string> create(const std::string &str) {
        for(auto &i : string::pool) {
            if(str == *i.first) {
                i.second++;
                return i.first;
            }
        }
        string::pool.emplace_back(std::make_pair(std::make_shared<std::string>(str), 1));
        return string::pool.back().first;
    }
    static void deleteString(const std::shared_ptr<const std::string> str) {
        auto iterator = string::pool.begin();
        while(iterator != string::pool.end()) {
            if(str == iterator->first) {
                iterator->second--;
                if(iterator->second == 0) { string::pool.erase(iterator); }
                return;
            }
            ++iterator;
        }
    }
};
std::vector<std::pair<std::shared_ptr<const std::string>, i64>> string::pool{};

/**
 * This function is a formatter for Token using fmt.
 * \cond
 */
template <> struct fmt::formatter<string> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
    template <typename FormatContext> auto format(const string &str, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "{}", std::string(str));
    }
};
/** \endcond */