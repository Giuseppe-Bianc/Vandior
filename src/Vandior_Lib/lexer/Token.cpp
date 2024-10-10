#include "Vandior/lexer/Token.hpp"
// NOLINTBEGIN(*-include-cleaner)
namespace vnd {
    std::string Token::to_string() const {
        if(_type == eofTokenType) { return FORMAT("(type: {}, source location:{})", _type, _sourceLocation); }
        return FORMAT("(type: {}, value: '{}', source location:{})", _type, _value, _sourceLocation);
    }

    std::string Token::compat_to_string() const {
        if(_type == eofTokenType) { return FORMAT("(typ: {:c}, sl:{:c})", _type, _sourceLocation); }
        return FORMAT("(typ: {:c}, val: '{}', sl:{:c})", _type, _value, _sourceLocation);
    }
}  // namespace vnd
// NOLINTEND(*-include-cleaner)
