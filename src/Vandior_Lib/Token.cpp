#include "Vandior/Token.hpp"
std::string Token::to_string() const { return FORMAT("(type: {}, value: {})", _type, _value); }  // NOLINT(*-include-cleaner)
