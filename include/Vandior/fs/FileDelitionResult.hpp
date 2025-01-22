// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "OSOperationResult.hpp"

DISABLE_WARNINGS_PUSH(4820)

namespace vnd {

    class FileDelitionResult : public OSOperationResult {
    public:
        using OSOperationResult::OSOperationResult;
        /**
         * @brief Deletes a file at the specified path.
         * @param filePath The path of the file to delete.
         * @return A FolderDeletionResult object indicating the result of the operation.
         */
        [[nodiscard]] static auto deleteFile(const fs::path &filePath) -> FileDelitionResult {
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
    };

}  // namespace vnd

DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner)