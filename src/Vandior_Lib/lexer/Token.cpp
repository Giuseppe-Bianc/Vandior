#include "Vandior/lexer/Token.hpp"
// NOLINTBEGIN(*-include-cleaner)
namespace vnd {
    std::string Token::to_string() const {
        if(_type == eofTokenType) { return FORMAT("(type: {}, source location:{})", _type, _sourceLocation); }
        return FORMAT("(type: {}, value: '{}', source location:{})", _type, _value, _sourceLocation);
    }

    std::string Token::compat_to_string() const {
        if(_type == eofTokenType) { return FORMAT("(typ: {}, sl:{})", comp_tokType(_type), _sourceLocation.compat_to_string()); }
        return FORMAT("(typ: {}, val: '{}', sl:{})", comp_tokType(_type), _value, _sourceLocation.compat_to_string());
    }
}  // namespace vnd
// NOLINTEND(*-include-cleaner)
