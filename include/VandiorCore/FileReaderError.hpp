//
// Created by gbian on 16/01/2024.
//
// NOLINTBEGIN(*-easily-swappable-parameters, *-include-cleaner)
#pragma once

#include "headersCore.hpp"

class FileReadError final : public std::runtime_error {
public:
    explicit FileReadError(const std::string &message) : std::runtime_error(message) {}
};

#define FILEREADEREERRORF(...) FileReadError(FORMAT(__VA_ARGS__))
// NOLINTEND(*-easily-swappable-parameters, *-include-cleaner)
