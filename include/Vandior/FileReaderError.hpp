//
// Created by gbian on 16/01/2024.
//
// NOLINTBEGIN(*-easily-swappable-parameters)
#pragma once

#include "headers.hpp"

class FileReadError final : public std::runtime_error {
public:
    explicit FileReadError(const std::string_view &message)
      : std::runtime_error(message.data()) {}
};

#define FILEREADEREERRORF(...) FileReadError(FORMAT(__VA_ARGS__))
// NOLINTEND(*-easily-swappable-parameters)
