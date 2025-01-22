//
// Created by gbian on 15/05/2024.
//
// NOLINTBEGIN(*-include-cleaner,  hicpp-signed-bitwise)
#pragma once

#include "FileReaderError.hpp"
#include "headersCore.hpp"
#include "timer/Timer.hpp"

namespace vnd {

    // Utility function to safely open a file and ensure correct handling of exceptions.
    inline auto openFile(const fs::path &filePath) -> std::ifstream {
        std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
        if(!fileStream.is_open()) { throw FILEREADEREERRORF("Unable to open file: {}", filePath.string()); }

        fileStream.exceptions(std::ios::failbit | std::ios::badbit);
        return fileStream;
    }

    // Function to read from file, safely and efficiently
    inline auto readFromFile(const std::string_view filename) -> std::string {
        static std::mutex fileReadMutex;
        const std::scoped_lock lock(fileReadMutex);  // Ensure thread safety
        const auto filePath = fs::path(filename);
        if(!fs::exists(filePath)) { throw FILEREADEREERRORF("File not found: {}", filePath); }
        if(!fs::is_regular_file(filePath)) { throw FILEREADEREERRORF("Path is not a regular file: {}", filePath); }

#ifdef INDEPT
        // Optional timer for performance measurement.
        const AutoTimer timer(FORMAT("Reading file {}", filename));
#endif

        // Ensure the file is properly opened and manage it with RAII.
        auto fileStream = openFile(filePath);

        try {
            std::ostringstream buffer;
            buffer << fileStream.rdbuf();
            return buffer.str();
        } catch(const std::ios_base::failure &e) {
            throw FILEREADEREERRORF("Unable to read file: {}. Reason: {}", filePath, e.what());
        } catch(const std::exception &e) {
            throw FILEREADEREERRORF("An error occurred while reading the file: {}. Reason: {}", filePath, e.what());
        }
    }
}  // namespace vnd

// NOLINTEND(*-include-cleaner,  hicpp-signed-bitwise)