//
// Created by gbian on 16/01/2024.
//

#pragma once

#include <Vandior/headers.hpp>

class FileReadError final : public std::runtime_error {
public:
    explicit FileReadError(const std::string_view &message)  // NOLINT(*-easily-swappable-parameters)
      : std::runtime_error(message.data()) {}
};

#define FILEREADEREERRORF(...) FileReadError(FORMAT(__VA_ARGS__))
