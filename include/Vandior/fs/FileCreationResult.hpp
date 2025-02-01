// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "OSOperationResult.hpp"

DISABLE_WARNINGS_PUSH(4820)

namespace vnd {

    class FileCreationResult : public OSOperationResult {
    public:
        using OSOperationResult::OSOperationResult;
        /**
         * @brief Create a file at the specified path.
         * @param path The path of the file to creat.
         * @param fileName the name of the file.
         * @param fileContent the content of the file.
         * @return A FolderCreationResult object indicating the result of the operation.
         */
        [[nodiscard]] static auto createFile(const fs::path &path, const std::string_view &fileName, const std::stringstream &fileContent)
            -> FileCreationResult {
            const auto filePath = path / fileName;
            return createFileFromPath(filePath, fileContent);
        }
        /**
         * @brief Create a file at the specified path.
         * @param filePath The path of the file create.
         * @param fileContent the content of the file.
         * @return A FolderCreationResult object indicating the result of the operation.
         */
        [[nodiscard]] static auto createFileFromPath(const fs::path &filePath, const std::stringstream &fileContent) -> FileCreationResult {
            try {
                std::ofstream outfile(filePath);
                if(!outfile.is_open()) {
                    LERROR("Failed to open file {}", filePath);
                    return {false, filePath};
                }
                outfile << fileContent.rdbuf();
                outfile.close();
                if(fs::exists(filePath)) {
#ifdef INDEPT
                    LINFO("File '{}' created successfully.", filePath);
#endif
                    return {true, filePath};
                } else {
                    LERROR("Failed to create '{}'", filePath);
                }
                return {false, filePath};
            } catch(const fs::filesystem_error &e) {
                LERROR("Filesystem error while creating file '{}': {}", filePath, e.what());
                return {false, filePath};
            } catch(const std::exception &e) {
                LERROR("Exception while creating file '{}': {}", filePath, e.what());
                return {false, filePath};
            } catch(...) {
                LERROR("An unknown error occurred while creating file '{}'.", filePath);
                return {false, filePath};
            }
        }
    };
}  // namespace vnd

DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner)