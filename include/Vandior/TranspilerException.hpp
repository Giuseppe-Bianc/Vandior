#pragma once
#include "Instruction.hpp"
#include "Log.hpp"
#include <stdexcept>

namespace vnd {
    /**
     * @brief Custom exception class for transpiler errors.
     * This exception is thrown when a transpiler encounters an error during the conversion process.
     */
    class TranspilerException : public std::runtime_error {
    public:
        /**
         * @brief Constructor for TranspilerException.
         * @param msg The error message.
         * @param instruction The problematic instruction causing the error.
         */
        TranspilerException(const std::string &msg, const Instruction &instruction)
          : std::runtime_error(FORMAT("{}\n{}", instruction, msg)) {}
    };

}  // namespace vnd
#define TRANSPILER_EXCEPTIONF(instruction, ...) TranspilerException(FORMAT(__VA_ARGS__), instruction)