/*
 * Created by gbian on 06/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */

#include "Vandior/lexer/ErrorHandler.hpp"

namespace vnd {

    ErrorHandler::ErrorHandler(std::string_view input, std::string_view filename, size_t line, size_t column)
      : _input(input), _filename(filename), _line(line), _column(column), _currentPos(0) {}

    void ErrorHandler::setCurrentPosition(size_t pos) { _currentPos = pos; }

    size_t ErrorHandler::findLineStart() const noexcept {
        size_t pos = _currentPos;
        while(pos > 0 && _input[pos - 1] != '\n') { --pos; }
        return pos;
    }

    size_t ErrorHandler::findLineEnd() const noexcept {
        size_t pos = _currentPos;
        while(pos < _input.size() && _input[pos] != '\n') { ++pos; }
        return pos;
    }

    std::string ErrorHandler::extractContext(size_t lineStart, size_t lineEnd) const {
        return std::string(_input.substr(lineStart, lineEnd - lineStart));
    }

    std::string ErrorHandler::getContextLine(size_t lineStart, size_t lineEnd) const { return extractContext(lineStart, lineEnd) + "\n"; }

    std::string ErrorHandler::getHighlighting(size_t lineStart, size_t lineEnd, std::string_view value) const {
        const auto temtp_val = extractContext(lineStart, lineEnd);
        auto tabs_section = extractTabs(temtp_val);
        if(const auto pos = temtp_val.find(value); pos != std::string::npos) {
            const auto val_len = value.length();
            if(pos == 0) [[unlikely]] {
                return FORMAT("{}{: ^{}}{:^{}}{}", tabs_section, "", pos, "^", val_len, NEWL);
            } else {
                return FORMAT("{}{: ^{}}{:^{}}{}", tabs_section, "", pos - 1, "^", val_len, NEWL);
            }
        }
        return FORMAT("{:^{}}{}", "^", (lineEnd - lineStart), NEWL);
    }

}  // namespace vnd