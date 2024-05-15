//
// Created by gbian on 15/05/2024.
//

#pragma once

#include "FileReaderError.hpp"
#include "Timer.hpp"
#include "headers.hpp"

namespace vnd {
    inline auto readFromFile(const std::string &filename) -> std::string {
        const auto &filePath = fs::path(filename);
        if(!fs::exists(filePath)) { throw FILEREADEREERRORF("File not found: {}", filePath); }
        if(!fs::is_regular_file(filePath)) { throw FILEREADEREERRORF("Path is not a regular file: {}", filePath); }

        AutoTimer timer(FORMAT("reading file {}", filename));
        std::stringstream buffer;
        // NOLINTNEXTLINE(*-include-cleaner,  hicpp-signed-bitwise)
        if(std::ifstream fileStream{filePath, std::ios::in | std::ios::binary}; fileStream.is_open()) {
            // Ensure
            // that the file is opened securely
            fileStream.exceptions(std::ios::failbit | std::ios::badbit);  // NOLINT(hicpp-signed-bitwise)

            try {
                buffer << fileStream.rdbuf();
            } catch(const std::ios_base::failure &e) { throw FILEREADEREERRORF("Unable to read file: {}. Reason: {}", filePath, e.what()); }
        } else {
            // Handle the case when the file cannot be opened,
            // You might throw an exception or return an error indicator
            throw FILEREADEREERRORF("Unable to open file: {}", filePath);
        }

        // Extract the content as a string
        return buffer.str();
    }
}  // namespace vnd