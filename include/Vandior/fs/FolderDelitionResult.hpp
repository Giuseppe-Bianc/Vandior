// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "FileDelitionResult.hpp"
#include "OSOperationResult.hpp"

DISABLE_WARNINGS_PUSH(4820)

namespace vnd {
    class FolderDeletionResult : public OSOperationResult {
    public:
        using OSOperationResult::OSOperationResult;

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
                                auto unused2 = FileDelitionResult::deleteFile(entry.path());
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
    };

}  // namespace vnd

DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner)