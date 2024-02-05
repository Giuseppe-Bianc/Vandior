#pragma once
#include "Token.hpp"
#include <stdexcept>
namespace vnd {
    /**
     * @brief Custom exception class for representing errors related to instructions.
     *
     * This exception is derived from std::runtime_error and is used to indicate errors
     * related to instructions during the execution of a program.
     */
    class InstructionException : public std::runtime_error {
    public:
        /**
         * @brief Constructs an InstructionException with information from a Token.
         *
         * @param token The Token associated with the exception.
         *
         * This constructor takes a Token object representing the location or context
         * of the error and constructs an InstructionException with a meaningful error message.
         */
        explicit InstructionException(const Token &token);
    };
}  // namespace vnd
