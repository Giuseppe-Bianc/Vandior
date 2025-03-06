/*
 * Created by gbian on 06/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters)
#pragma once

#include "../headers.hpp"

namespace vnd {

    class ErrorHandler {
    public:
        // Costruttore: riceve il testo di input, il nome del file, la linea e la colonna corrente.
        ErrorHandler(std::string_view input, std::string_view filename, size_t line, size_t column);

        // Setter per aggiornare la posizione corrente (da chiamare prima di gestire l'errore)
        void setCurrentPosition(size_t pos);

        // Funzione template per gestire l'errore. (Deve essere definita qui per via della natura template)
        template <typename T> [[noreturn]] void handleError(const T &value, const std::string_view &errMsg) {
            const auto lineStart = findLineStart();
            const auto lineEnd = findLineEnd();
            const auto contextLine = getContextLine(lineStart, lineEnd);
            const auto highlighting = getHighlighting(lineStart, lineEnd, std::string_view(value));
            const auto errorMessage = getErrorMessage(value, errMsg, contextLine, highlighting);
            throw std::runtime_error(errorMessage);
        }

    private:
        // Funzioni non-template, la cui implementazione è nel file .cpp
        [[nodiscard]] size_t findLineStart() const noexcept;
        [[nodiscard]] size_t findLineEnd() const noexcept;
        [[nodiscard]] std::string extractContext(size_t lineStart, size_t lineEnd) const;
        [[nodiscard]] std::string getContextLine(size_t lineStart, size_t lineEnd) const;
        [[nodiscard]] std::string getHighlighting(size_t lineStart, size_t lineEnd, std::string_view value) const;

        // Funzione template per generare il messaggio d'errore
        template <typename T>
        std::string getErrorMessage(const T &value, const std::string_view &errMsg, const std::string &contextLine,
                                    const std::string &highlighting);

        std::string_view _input;
        std::string_view _filename;
        size_t _line;
        size_t _column;
        size_t _currentPos;
    };

    // Definizione della funzione template getErrorMessage
    template <typename T>
    std::string ErrorHandler::getErrorMessage(const T &value, const std::string_view &errMsg, const std::string &contextLine,
                                              const std::string &highlighting) {
        std::ostringstream errorMessageStream;
        errorMessageStream << FORMAT("{} '{}' (line {}, column {}):{}", errMsg, value, _line, _column, NEWL);
        errorMessageStream << FORMAT("Context: {}", NEWL);
        errorMessageStream << contextLine;
        errorMessageStream << highlighting;
        return errorMessageStream.str();
    }
}  // namespace vnd

// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters)