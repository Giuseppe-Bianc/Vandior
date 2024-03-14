// NOLINTBEGIN(*-include-cleaner,*-identifier-length)
#include "Vandior/Scope.hpp"
#include <Vandior/Log.hpp>
/**
 * This macro disable some msvc warnigs.
 * \cond
 */
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)
/** \endcond */

namespace vnd {
    // NOLINTBEGIN
    std::vector<std::string> Scope::_signedTypes = {"i8", "i16", "i32", "i64"};
    std::vector<std::string> Scope::_unsignedTypes = {"u8", "u16", "u32", "u64"};
    std::vector<std::string> Scope::_floatingTypes = {"f32", "f64"};
    std::vector<std::string> Scope::_complexTypes = {"c32", "c64"};
    std::vector<std::string> Scope::_primitiveTypes = {"i8",  "i16", "i32", "i64",  "u8",   "u16",   "u32",
                                                       "u64", "f32", "f64", "c32", "c64", "char", "bool", "string"};
    // NOLINTEND

    Scope::Scope(std::shared_ptr<Scope> parent, const ScopeType &type) noexcept : _parent(std::move(parent)), _type(type) {
#ifdef PRINT_SCOPE_TYPE
        LINFO(_type);
#endif  // PRINT_SCOPE_TYPE
    }

    std::shared_ptr<Scope> Scope::create(std::shared_ptr<Scope> parent, const ScopeType &type) noexcept {
        return std::make_shared<Scope>(Scope{std::move(parent), type});
    }

    std::shared_ptr<Scope> Scope::createMain() noexcept {
        auto mainScope = std::make_shared<Scope>(Scope{nullptr, ScopeType::GLOBAL_SCOPE});
        mainScope->addType("void");
        for(auto &type : Scope::_primitiveTypes) { mainScope->addType(type); }
        mainScope->addType("Object");
        mainScope->addType("Derived", {"Object"});
        mainScope->addVariable("Object.a", "i32", false);
        mainScope->addVariable("Object.test", "f32", true);
        mainScope->addVariable("Object.s", "string", false);
        mainScope->addVariable("Derived._derivedProperty", "bool", false);
        mainScope->addVariable("Derived.obj", "Object", false);
        mainScope->addConstant("Object.c", "i32", "2");
        mainScope->addConstant("Derived._derivedConst", "bool", "true");
        mainScope->addFun("print", FunType::create("void", {"string", "any..."}, {}, {}));
        mainScope->addFun("println", FunType::create("void", {"string", "any..."}, {}, {}));
        mainScope->addFun("readLine", FunType::create("string", {}, {}, {}));
        mainScope->addFun("_test", FunType::create("i32", {}, {}, {}));
        mainScope->addFun("testPar", FunType::create("i32", {"i32", "i32"}, {}, {}));
        mainScope->addFun("testPar", FunType::create("i64", {"string"}, {}, {}));
        mainScope->addFun("max", FunType::create("i32 f32", {"f32[]"}, {}, {}));
        mainScope->addFun("arrayTest", FunType::create("i32[]", {}, {}, {}));
        mainScope->addFun("createObject", FunType::create("Object", {}, {}, {}));
        mainScope->addFun("Object", FunType::create("Object", {}, {}, {}, true));
        mainScope->addFun("Derived", FunType::create("Derived", {}, {}, {}, true));
        mainScope->addFun("Derived", FunType::create("Derived", {"Object", "bool"}, {}, {}, true));
        mainScope->addFun("createDerived", FunType::create("Derived", {}, {}, {}));
        mainScope->addFun("vnd::vector.add", FunType::create("void", {"T"}, {{"T", "any"}}, {}));
        mainScope->addFun("vnd::vector.addVector", FunType::create("void", {"T[]"}, {{"T", "any"}}, {}));
        mainScope->addFun("vnd::vector.addAll", FunType::create("void", {"T..."}, {{"T", "any"}}, {}));
        mainScope->addFun("vnd::vector.size", FunType::create("i64", {}, {{"T", "any"}}, {}));
        mainScope->addFun("vnd::array.size", FunType::create("i64", {}, {{"T", "any"}}, {}));
        mainScope->addFun("string.size", FunType::create("i64", {}, {}, {}));
        mainScope->addFun("string.toI32", FunType::create("i32", {}, {}, {}));
        mainScope->addFun("string.toF32", FunType::create("f32", {}, {}, {}));
        mainScope->addFun("string.toC32", FunType::create("c32", {}, {}, {}));
        mainScope->addFun("Object.f", FunType::create("f64", {"f64"}, {}, {}));
        mainScope->addFun("Object.fs", FunType::create("string", {}, {}, {}));
        mainScope->addFun("Derived.derivedFun", FunType::create("bool", {"Object"}, {}, {}));
        mainScope->addFun("Derived.object", FunType::create("Object", {}, {}, {}));
        return mainScope;
    }

    bool Scope::isInteger(const std::string &type) noexcept {
        return std::ranges::find(Scope::_signedTypes, type) != Scope::_signedTypes.end() ||
               std::ranges::find(Scope::_unsignedTypes, type) != Scope::_unsignedTypes.end();
    }

    bool Scope::isComplex(const std::string &type) noexcept {
        return std::ranges::find(Scope::_complexTypes, type) != Scope::_complexTypes.end();
    }

    bool Scope::isNumber(const std::string &type) noexcept {
        return Scope::isInteger(type) || Scope::isComplex(type) ||
               std::ranges::find(Scope::_floatingTypes, type) != Scope::_floatingTypes.end();
    }

    bool Scope::isPrimitive(const std::string &type) noexcept {
        return std::ranges::find(Scope::_primitiveTypes, type) != Scope::_primitiveTypes.end();
    }

    bool Scope::checkVector(std::string &type) noexcept {
        if(type == "string") {
            type = "char";
            return true;
        }
        if(type.back() != ']') { return false; }
        while(type.back() != '[') { type.pop_back(); }
        type.pop_back();
        return true;
    }

    std::string Scope::getTypeValue(const std::string &type) noexcept {
        auto pos = type.find('[');
        if(pos == std::string::npos) { pos = type.size(); }
        auto typeValue = type.substr(0, pos);
        if(!Scope::isPrimitive(typeValue)) { typeValue = FORMAT("std::shared_ptr<{}>", typeValue); }
        if(pos != type.size()) {
            std::string size;
            for(auto c : type) {
                if(c == '[') {
                    size.clear();
                } else if(c == ']') {
                    if(size.empty()) {
                        typeValue = FORMAT("vnd::vector<{}>", typeValue);
                    } else {
                        typeValue = FORMAT("vnd::array<{}, {}>", typeValue, std::stoi(size));
                    }
                } else if(std::isdigit(c)) {
                    size += c;
                }
            }
        }
        return typeValue;
    }

    std::string Scope::getKey(const std::string &type, const std::string_view &identifier) noexcept {
        if(type.empty()) { return std::string(identifier); }
        return FORMAT("{}.{}", type, identifier);
    }

    std::string Scope::getType(const std::string &type) noexcept {
        if(type.ends_with("[]")) [[unlikely]] { return FORMAT("vnd::vector<{}>", type.substr(0, type.size() - 2)); }
        if(type.ends_with("]")) [[unlikely]] { return FORMAT("vnd::array<{}>", type.substr(0, type.find_last_of('['))); }
        return type;
    }

    std::string Scope::getParamType(const std::string &param,
                                    const std::vector<std::pair<std::string, std::string>> &typeGeneric) noexcept {
        size_t pos = param.find('[');
        auto it = std::ranges::find_if(typeGeneric, [&param, pos](const std::pair<std::string, std::string> &element) {
            if(pos == std::string::npos) { return param == element.first; }
            return param.substr(0, pos) == element.first;
        });
        if(it == typeGeneric.end()) { return param; }
        if(pos == std::string::npos) { return it->second; }
        return it->second + param.substr(pos, param.size() - pos);
    }

    std::shared_ptr<Scope> Scope::getParent() const noexcept { return _parent; }

    ScopeType Scope::getType() const noexcept { return _type; }

    void Scope::removeParent() noexcept { _parent = nullptr; }

    void Scope::addType(const std::string_view &type, const std::vector<std::string> &assignable) noexcept {
        _types[std::string(type)] = assignable;
    }

    void Scope::addConstant(const std::string_view &identifier, const std::string_view &type, const std::string &value) noexcept {
        _consts[std::string{identifier}] = std::make_pair(type, value);
    }

    void Scope::addVariable(const std::string_view &identifier, const std::string_view &type, const bool isVal) noexcept {
        if(isVal) { _vals[std::string{identifier}] = type; }
        _vars[std::string{identifier}] = type;
    }

    void Scope::addFun(const std::string_view identifier, const FunType &fun) noexcept {
        auto key = std::string{identifier};
        if(!_funs.contains(key)) { _funs.try_emplace(key); }
        _funs[key].emplace_back(fun);
    }

    bool Scope::isGlobalScope() const noexcept { return _parent == nullptr && _type == ScopeType::GLOBAL_SCOPE; }

    bool Scope::checkType(const std::string_view type) const noexcept {  // NOLINT(*-no-recursion)
        if(contains_key(_types, type)) { return true; }
        if(_parent) { return _parent->checkType(type); }
        return false;
    }

    template <typename KeyType, typename ValueType>
    bool Scope::contains_key(const std::unordered_map<KeyType, ValueType> &map, std::string_view key) const noexcept {
        return map.contains(std::string(key));
    }

    // NOLINTNEXTLINE
    std::pair<bool, bool> Scope::checkVariable(const std::string_view identifier, const bool shadowing) const noexcept {
        if(contains_key(_vars, identifier) || contains_key(_vals, identifier) || contains_key(_consts, identifier)) {
            return {true, shadowing};
        }
        if(_parent) { return _parent->checkVariable(identifier, true); }
        return {false, false};
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::string_view Scope::getVariableType(const std::string &type, const std::string_view &identifier) const noexcept {
        auto key = Scope::getKey(type, identifier);
        if(_vars.contains(key)) { return _vars.at(key); }
        if(_vals.contains(key)) { return _vals.at(key); }
        if(_consts.contains(key)) { return _consts.at(key).first; }
        if(_types.contains(type)) {
            for(const std::string &i : _types.at(type)) {
                std::string_view result = getVariableType(i, identifier);
                if(!result.empty()) { return result; }
            }
        }
        if(_parent) { return _parent->getVariableType(type, identifier); }
        return "";
    }

    // NOLINTNEXTLINE(*-no-recursion,readability-function-cognitive-complexity)
    std::tuple<std::string, bool, std::optional<size_t>> Scope::getFunType(const std::string &type, const std::string_view &identifier,
                                                                           const std::vector<Expression> &expressions) const noexcept {
        bool found = false;
        for(const auto &i : getFuns(Scope::getType(type), identifier)) {
            auto params = i.getParams();
            std::optional<size_t> variadic;
            found = true;
            if(!params.empty() && params.back().ends_with("...")) {
                variadic = params.size() - 1;
                if(expressions.size() == params.size() - 1) {
                    params.pop_back();
                } else if(expressions.size() >= params.size()) {
                    params.back().erase(params.back().end() - 3, params.back().end());
                    std::string lastPar = params.back();
                    while(params.size() < expressions.size()) { params.push_back(lastPar); }
                }
            }
            if(params.size() != expressions.size()) [[likely]] {
                found = false;
            } else [[unlikely]] {
                for(size_t par = 0; par != expressions.size(); par++) {
                    std::string paramType = Scope::getParamType(params.at(par), i.getTypeGeneric());
                    if(auto result = canAssign(paramType, expressions.at(par).getType()); !result.first) { found = false; }
                }
            }
            if(found) { return {i.getReturnType(), i.isConstructor(), variadic}; }
        }
        if(_types.contains(type)) {
            for(const std::string &i : _types.at(type)) {
                auto [result, constructor, variadic] = getFunType(i, identifier, expressions);
                if(!result.empty()) { return {result, constructor, variadic}; }
            }
        }
        if(_parent) { return _parent->getFunType(type, identifier, expressions); }
        return {"", false, {}};
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::string Scope::getConstValue(const std::string &type, const std::string_view &identifier) const noexcept {
        if(auto key = Scope::getKey(type, identifier); _consts.contains(key)) { return _consts.at(key).second; }
        if(_types.contains(type)) {
            for(const std::string &i : _types.at(type)) {
                std::string result = getConstValue(i, identifier);
                if(!result.empty()) { return result; }
            }
        }
        if(_parent) { return _parent->getConstValue(type, identifier); }
        return "";
    }

    // NOLINTNEXTLINE(*-no-recursion)
    bool Scope::isConstant(const std::string &type, const std::string_view &identifier) const noexcept {
        if(auto key = Scope::getKey(type, identifier); _consts.contains(key) || _vals.contains(key)) { return true; }
        if(_types.contains(type)) {
            for(const std::string &i : _types.at(type)) {
                bool result = isConstant(i, identifier);
                if(result) { return result; }
            }
        }
        if(_parent) { return _parent->isConstant(type, identifier); }
        return false;
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::pair<bool, bool> Scope::canAssign(const std::string &left, const std::string &right) const noexcept {
        if(left == "any" || left == right) { return {true, false}; }
        if((Scope::isNumber(left) && Scope::isNumber(right))) {
            if(left.at(0) == right.at(0) && std::stoi(left.substr(1)) < std::stoi(right.substr(1))) {
                return {!Scope::isInteger(left), true};
            }
            if(Scope::isComplex(right)) { return {Scope::isComplex(left), false}; }
            return {true, false};
        }
        if(right == "nullptr") { return {!Scope::isPrimitive(left), false}; }
        if(right == "[]" && left.ends_with(']')) { return {true, false}; }
        if(std::pair<std::string, std::string> types = {left, right}; (types.first.ends_with("[]") || types.second.ends_with("[]")) &&
                                                                      Scope::checkVector(types.first) && Scope::checkVector(types.second)) {
            return canAssign(types.first, types.second);
        }
        if(_types.contains(right)) {
            for(const std::string &i : _types.at(right)) {  // NOLINT(*-use-anyofallof)
                if(auto result = canAssign(left, i); result.first) { return result; }
            }
        }
        if(_parent) { return _parent->canAssign(left, right); }
        return {};
    }

    std::vector<FunType> Scope::getFuns(const std::string &type, const std::string_view &identifier) const noexcept {
        std::vector<FunType> result;
        std::vector<std::string> typeSpecialized;
        std::string typePrefix = type;
#ifdef __llvm__
        const bool typContainsMin = type.find('<') != std::string::npos;
#else
        const bool typContainsMin = type.contains('<');
#endif
        if(typContainsMin) {
            size_t pos = type.find('<');
            std::string currentParam;
            for(const char c : type.substr(pos + 1, type.find_last_of('>') - 1)) {
                switch(c) {
                case '<':
                    [[fallthrough]];
                case '>':
                    continue;
                case ',':
                    typeSpecialized.emplace_back(currentParam);
                    currentParam.clear();
                    continue;
                default:
                    currentParam += c;
                    continue;
                }
            }
            typeSpecialized.emplace_back(currentParam);
            typePrefix = type.substr(0, pos);
        }
        std::string key = getKey(typePrefix, identifier);
        if(!_funs.contains(key)) { return {}; }
        for(const auto &fun : _funs.at(key)) {
            auto [resultFun, ok] = specializeFun(fun, typeSpecialized);
            if(ok) { result.emplace_back(resultFun); }
        }
        return result;
    }

    std::pair<FunType, bool> Scope::specializeFun(const FunType &fun, const std::vector<std::string> &typeSpecialized) const noexcept {
        std::vector<std::pair<std::string, std::string>> typeGeneric = fun.getTypeGeneric();
        std::vector<std::pair<std::string, std::string>> resultGeneric;
        // TODO: fix this if statement
        if(typeGeneric.size() != typeGeneric.size()) { return {FunType::createEmpty(), false}; }
        auto it_specialized = typeSpecialized.begin();
        for(auto &[key, value] : typeGeneric) {
            if(it_specialized == typeSpecialized.end()) { return {FunType::createEmpty(), false}; }
            if(auto result = canAssign(value, *it_specialized); !result.first) { return {FunType::createEmpty(), false}; }
            value = *it_specialized;
            ++it_specialized;
        }

        // Check if typeSpecialized still has remaining elements
        if(it_specialized != typeSpecialized.end()) { return {FunType::createEmpty(), false}; }
        return {FunType::create(fun.getReturnType(), fun.getParams(), typeGeneric, fun.getFuncGeneric(), fun.isConstructor()), true};
    }

}  // namespace vnd

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner, *-identifier-length)
