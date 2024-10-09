//
// Created by potex02 on 11/05/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "../headers.hpp"

namespace vnd {
    /**
     * @brief A class representing an exception of the parser.
     */
    class ParserException final : public std::runtime_error {
    public:
        /**
         * @brief Creates a ParserException from an unexpected token.
         * @param token the unexpected token found during the parsing.
         */
        explicit ParserException(const Token &token) : std::runtime_error(FORMAT("Unexpected token: {}", token)){};
    };
}  // namespace vnd

// NOLINTEND(*-include-cleaner)