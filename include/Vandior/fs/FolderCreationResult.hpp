// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "OSOperationResult.hpp"

DISABLE_WARNINGS_PUSH(4820)

namespace vnd {

    /**
     * @class FolderCreationResult
     * @brief Represents the result of a folder creation operation.
     */
    class FolderCreationResult : public OSOperationResult {
    public:
        using OSOperationResult::OSOperationResult;

        [[nodiscard]] static auto createFolder(std::string_view folderName, const fs::path &parentDir) -> FolderCreationResult {
            // Validate the parameters
            if(folderName.empty()) {
                LERROR("Invalid parameters: parentPath or folderName is empty.");
                return {false, {}};
            }

            auto newDirPath = parentDir / folderName;
            if(fs::exists(newDirPath)) {
#ifdef INDEPT
                LWARN("The folder already exists: {}", newDirPath);
#endif
                return {true, newDirPath};  // Return true since the folder already exists
            }

            // Create the new directory
            std::error_code error_codec;
            if(fs::create_directories(newDirPath, error_codec)) {
#ifdef INDEPT
                LINFO("Folder '{}' created successfully at '{}'", folderName, newDirPath);
#endif
                return {true, newDirPath};
            } else {
                if(error_codec) {
                    // Log the error with specific details
                    LERROR("Failed to create folder '{}' at '{}': {} (Error code: {})", folderName, newDirPath, error_codec.message(),
                           error_codec.value());
                } else {
                    LERROR("Failed to create folder '{}' at '{}' for an unknown error", folderName, newDirPath);
                }
                return {false, {}};
            }
        }

        [[nodiscard]] static auto createFolderNextToFile(const fs::path &filePath, std::string_view folderName) -> FolderCreationResult {
            try {
                // Get the parent directory of the file
                auto parentDir = filePath.parent_path();

                // Construct the path for the new directory
                return createFolder(folderName, parentDir);
            } catch(const fs::filesystem_error &e) {
                // Handle specific filesystem errors
                LERROR("Filesystem error: {} (Path1: '{}', Path2: '{}', Error code: {})", e.what(), e.path1(), e.path2(), e.code().value());
                return {false, {}};
            } catch(const std::exception &e) {
                // Handle all other standard exceptions
                LERROR("Exception occurred: {}", e.what());
                return {false, {}};
            } catch(...) {
                // Handle any other types of exceptions
                LERROR("An unknown error occurred while creating the folder.");
                return {false, {}};
            }
        }
    };

}  // namespace vnd

DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner)