//
// Created by gbian on 15/05/2024.
//

#pragma once

#include "FileReaderError.hpp"
#include "headers.hpp"
#include "timer/Timer.hpp"

namespace vnd {
    inline auto readFromFile(const std::string &filename) -> std::string {
        static std::mutex fileReadMutex;
        std::scoped_lock lock(fileReadMutex);  // Ensure thread safety
        const auto &filePath = fs::path(filename);
        if(!fs::exists(filePath)) { throw FILEREADEREERRORF("File not found: {}", filePath); }
        if(!fs::is_regular_file(filePath)) { throw FILEREADEREERRORF("Path is not a regular file: {}", filePath); }

        AutoTimer timer(FORMAT("reading file {}", filename));
        std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
        if(!fileStream.is_open()) { throw FILEREADEREERRORF("Unable to open file: {}", filePath); }

        fileStream.exceptions(std::ios::failbit | std::ios::badbit);  // Ensure that the file is opened securely

        try {
            // Pre-allocate string to improve performance
            fileStream.seekg(0, std::ios::end);
            std::string buffer;
            buffer.reserve(fileStream.tellg());
            fileStream.seekg(0, std::ios::beg);

            buffer.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
            return buffer;
        } catch(const std::ios_base::failure &e) { throw FILEREADEREERRORF("Unable to read file: {}. Reason: {}", filePath, e.what()); }
    }
}  // namespace vnd
