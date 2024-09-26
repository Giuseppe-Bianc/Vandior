#pragma once
#include "basemath.cpp"
#include "container.hpp"
#include "headers.hpp"
#include "primitives.hpp"
#include "string.hpp"
#include <build/_deps/fmt-src/include/fmt/core.h>
#include <format>

#define FOR_LOOP(type, var, initial, final, step) for(type var = initial; (step > 0 ? var < final : var > final); var += step)

namespace vnd {
    std::unordered_map<std::string, std::any> tmp;
    vnd::vector<string> createArgs(int argc, char **argv) {
        vnd::vector<string> args;
        FOR_LOOP(size_t, i, 0, argc, 1) { args.add(std::string(argv[i])); }
        return args;
    }

}  // namespace vnd

i32 v_test() { return 0; }
i32 _testPar(i32 a, i32 b) { return a + b; }
i64 _testPar(string s) { return s.size(); }
class Object {
public:
    i32 getC() { return c; };
    i32 getA() { return a; };
    void setA(i32 _a) { a = _a; };
    f32 getTest() { return test; }
    string getS() { return s; }
    void setS(string _s) { s = _s; }
    virtual f64 f(f64 b) { return std::pow(b, 2); }
    virtual string fs() { return string(); }
    virtual string toString() {
        std::stringstream ss;
        ss << std::hex << std::setw(sizeof(uintptr_t) * 2) << std::setfill('0') << reinterpret_cast<std::uintptr_t>(this);
        std::string hexString = ss.str();
        hexString.erase(0, hexString.find_first_not_of('0'));
        return hexString;
    }

private:
    const i32 c = 2;
    const f32 test = 0;
    i32 a{};
    string s{};
};
class Derived : public Object {
public:
    Derived() : obj(std::make_shared<Object>()) {}
    Derived(std::shared_ptr<Object> _obj, bool derivedProperty) : _derivedProperty(derivedProperty), obj(std::make_shared<Object>()) {}
    bool get_derivedProperty() { return _derivedProperty; }
    void set_derivedProperty(bool derivedProperty) { _derivedProperty = derivedProperty; }
    std::shared_ptr<Object> getObj() { return obj; }
    void setObj(std::shared_ptr<Object> _obj) { obj = _obj; }
    bool get_derivedConst() { return _derivedConst; }
    virtual bool derivedFun(std::shared_ptr<Object> obj) { return obj->getS().empty(); }
    virtual std::shared_ptr<Object> object() { return std::make_shared<Object>(); }
    virtual string toString() { return string("Derived" + std::string(Object::toString())); }

private:
    const bool _derivedConst = true;
    bool _derivedProperty{};
    std::shared_ptr<Object> obj{};
};
template <typename... Args> inline void _print(string format, Args &&...args) {
    fmt::print(fmt::runtime(std::string(format)), std::forward<Args>(args)...);
}
template <typename... Args> inline void _println(string format, Args &&...args) {
    fmt::print(fmt::runtime(std::string(format) + "\n"), std::forward<Args>(args)...);
}
inline void _exit(i32 code) { std::exit(code); }
string _readLine() {
    std::string input;
    std::getline(std::cin, input);
    return string(input);
}
std::tuple<i64, f64> _max(vnd::vector<f64>) { return {0, 0}; }
vnd::vector<i32> _arrayTest() { return {1, 2, 3}; }
std::shared_ptr<Object> _createObject() { return std::make_shared<Object>(); }
std::shared_ptr<Derived> _createDerived() { return std::make_shared<Derived>(); }

/**
 * This function is a formatter for Token using fmt.
 * \cond
 */
template <> struct fmt::formatter<std::shared_ptr<Object>> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
    template <typename FormatContext> auto format(const std::shared_ptr<Object> &obj, FormatContext &ctx) {
        if(obj) {
            return fmt::format_to(ctx.out(), "{}", obj->toString());
        } else {
            return fmt::format_to(ctx.out(), "nullptr");
        }
    }
};

/** \endcond */