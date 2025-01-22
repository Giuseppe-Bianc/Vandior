// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "FolderOperationResult.hpp"

DISABLE_WARNINGS_PUSH(4820)

namespace vnd {
    class FolderDeletionResult : public FolderOperationResult {
    public:
        using FolderOperationResult::FolderOperationResult;

        /**
         * @brief Deletes a file at the specified path.
         * @param filePath The path of the file to delete.
         * @return A FolderDeletionResult object indicating the result of the operation.
         */
        [[nodiscard]] static auto deleteFile(const fs::path &filePath) -> FolderDeletionResult {
            try {
                if(fs::exists(filePath)) {
                    if(fs::is_regular_file(filePath)) {
                        fs::remove(filePath);
#ifdef INDEPT
                        LINFO("File '{}' deleted successfully.", filePath);
#endif
                        return {true, filePath};
                    } else {
                        LERROR("The path '{}' is not a file.", filePath);
                        return {false, filePath};
                    }
                } else {
                    LERROR("File '{}' does not exist.", filePath);
                    return {false, filePath};
                }
            } catch(const fs::filesystem_error &e) {
                LERROR("Failed to delete file '{}': {}", filePath, e.what());
                return {false, filePath};
            } catch(...) {
                LERROR("An unknown error occurred while deleting file '{}'.", filePath);
                return {false, filePath};
            }
        }

        /**
         * @brief Deletes a folder at the specified path, including its contents.
         * @param folderPath The path of the folder to delete.
         * @return A FolderDeletionResult object indicating the result of the operation.
         */
        [[nodiscard]] static auto deleteFolder(const fs::path &folderPath) -> FolderDeletionResult {
            try {
                if(fs::exists(folderPath)) {
                    if(fs::is_directory(folderPath)) {
                        // Recursively delete all contents of the folder
                        for(const auto &entry : fs::directory_iterator(folderPath)) {
                            if(fs::is_directory(entry)) {
                                auto unused = deleteFolder(entry.path());
                            } else {
                                auto unused2 = deleteFile(entry.path());
                            }
                        }
                        // Remove the folder itself
                        fs::remove(folderPath);
#ifdef INDEPT
                        LINFO("Folder '{}' deleted successfully.", folderPath);
#endif
                        return {true, folderPath};
                    } else {
                        LERROR("The path '{}' is not a folder.", folderPath);
                        return {false, folderPath};
                    }
                } else {
                    LERROR("Folder '{}' does not exist.", folderPath);
                    return {false, folderPath};
                }
            } catch(const fs::filesystem_error &e) {
                LERROR("Failed to delete folder '{}': {}", folderPath, e.what());
                return {false, folderPath};
            } catch(...) {
                LERROR("An unknown error occurred while deleting folder '{}'.", folderPath);
                return {false, folderPath};
            }
        }

    private:
        bool success_ = false;          ///< The success status of the folder or file deletion.
        std::optional<fs::path> path_;  ///< The path of the deleted folder or file.
    };

}  // namespace vnd

DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner)