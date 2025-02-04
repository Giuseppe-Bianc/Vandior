// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "Vandior/lexer/Token.hpp"

namespace vnd {

static inline constexpr auto icr = 'i';
static inline constexpr auto fcr = 'f';
// clang-format off
static inline constexpr std::array<std::pair<std::string_view, TokenType>, 14> multiCharOperators = {{
    {"+=", TokenType::PLUSEQUAL}, {"-=", TokenType::MINUSEQUAL}, {"*=", TokenType::STAREQUAL},
    {"/=", TokenType::DIVIDEEQUAL}, {"^=", TokenType::XOREQUAL}, {"%=", TokenType::PERCENTEQUAL},
    {"==", TokenType::EQUALEQUAL}, {">=", TokenType::GREATEREQUAL}, {"<=", TokenType::LESSEQUAL},
    {"!=", TokenType::NOTEQUAL}, {"&&", TokenType::ANDAND}, {"||", TokenType::OROR},
    {"++", TokenType::PLUSPLUS}, {"--", TokenType::MINUSMINUS}
}};
static inline constexpr std::array<std::pair<std::string_view, TokenType>, 15> typeArray = {{
    {"i8"sv, TokenType::TYPE_I8},   {"i16"sv, TokenType::TYPE_I16}, {"i32"sv, TokenType::TYPE_I32},   {"i64"sv, TokenType::TYPE_I64},       {"u8"sv, TokenType::TYPE_U8},
    {"u16"sv, TokenType::TYPE_U16}, {"u32"sv, TokenType::TYPE_U32}, {"u64"sv, TokenType::TYPE_U64},   {"f32"sv, TokenType::TYPE_F32},       {"f64"sv, TokenType::TYPE_F64},
    {"c32"sv, TokenType::TYPE_C32}, {"c64"sv, TokenType::TYPE_C64}, {"char"sv, TokenType::TYPE_CHAR}, {"string"sv, TokenType::TYPE_STRING}, {"bool"sv, TokenType::TYPE_BOOL}
}};
static inline constexpr std::array<std::pair<std::string_view, TokenType>, 15> keywordArray = {{
    {"main"sv, TokenType::K_MAIN}, {"var"sv, TokenType::K_VAR},{"val"sv, TokenType::K_VAR},
    {"const"sv, TokenType::K_VAR},{"if"sv, TokenType::K_IF}, {"while"sv, TokenType::K_WHILE},
    {"else"sv, TokenType::K_ELSE},{"for"sv, TokenType::K_FOR},{"break"sv, TokenType::K_BREAK},
    {"continue"sv, TokenType::K_BREAK}, {"fun"sv, TokenType::K_FUN}, {"return"sv, TokenType::K_RETURN},
    {"nullptr"sv, TokenType::K_NULLPTR},{"true"sv, TokenType::BOOLEAN},{"false"sv, TokenType::BOOLEAN}
}};

// clang-format on
}
// NOLINTEND(*-include-cleaner)