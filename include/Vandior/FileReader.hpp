//
// Created by gbian on 15/05/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "FileReaderError.hpp"
#include "headers.hpp"
#include "timer/Timer.hpp"

namespace vnd {
    inline auto readFromFile(const std::string &filename) -> std::string {
        // NOLINTBEGIN(*-include-cleaner,  hicpp-signed-bitwise)
        static std::mutex fileReadMutex;
        const std::scoped_lock lock(fileReadMutex);  // Ensure thread safety
        const auto &filePath = fs::path(filename);
        if(!fs::exists(filePath)) { throw FILEREADEREERRORF("File not found: {}", filePath); }
        if(!fs::is_regular_file(filePath)) { throw FILEREADEREERRORF("Path is not a regular file: {}", filePath); }

#ifdef INDEPT
        const AutoTimer timer(FORMAT("reading file {}", filename));
#endif
        std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
        if(!fileStream.is_open()) { throw FILEREADEREERRORF("Unable to open file: {}", filePath); }

        fileStream.exceptions(std::ios::failbit | std::ios::badbit);

        try {
            std::ostringstream buffer;
            buffer << fileStream.rdbuf();
            return buffer.str();
        } catch(const std::ios_base::failure &e) {
            throw FILEREADEREERRORF("Unable to read file: {}. Reason: {}", filePath, e.what());
        } catch(const std::exception &e) {
            throw FILEREADEREERRORF("An error occurred while reading the file: {}. Reason: {}", filePath, e.what());
        }
        // NOLINTEND(*-include-cleaner,  hicpp-signed-bitwise)
    }
}  // namespace vnd

// NOLINTEND(*-include-cleaner)