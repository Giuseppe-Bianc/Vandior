#include "Vandior/Token.hpp"
std::string Token::to_string() const { return FORMAT("(type: {}, value: {}, line: {}, column: {})", _type, _value, _line, _column); }  // NOLINT(*-include-cleaner)
