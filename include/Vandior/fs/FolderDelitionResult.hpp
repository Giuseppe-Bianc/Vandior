// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "FSConstats.hpp"

namespace vnd {
    class FolderDeletionResult {
    public:
        /**
         * @brief Checks if the folder deletion was successful.
         * @return True if the folder deletion was successful, false otherwise.
         */
        [[nodiscard]] constexpr bool success() const noexcept { return success_; }

        /**
         * @brief Returns the path of the deleted folder.
         * @return The path of the deleted folder.
         */
        [[nodiscard]] std::optional<fs::path> path() const { return path_; }

        /**
         * @brief Sets the success status of the folder deletion.
         * @param success The success status of the folder deletion.
         */
        void set_success(const bool success) noexcept { success_ = success; }

        /**
         * @brief Sets the path of the deleted folder.
         * @param path The path of the deleted folder.
         * @throws std::invalid_argument if the path is empty.
         */
        void set_path(const fs::path &path) {
            if(path.empty()) { throw std::invalid_argument("Path cannot be empty"); }
            path_ = path;
        }

        /**
         * @brief Default constructor for FolderDeletionResult.
         */
        FolderDeletionResult() = default;

        /**
         * @brief Constructs a FolderDeletionResult with the specified success status and path.
         * @param success The success status of the folder deletion.
         * @param path The path of the deleted folder.
         */
        FolderDeletionResult(const bool success, const fs::path &path) : success_{success}, path_{path} {}

        ~FolderDeletionResult() = default;

        friend void swap(FolderDeletionResult &lhs, FolderDeletionResult &rhs) noexcept {
            std::swap(lhs.success_, rhs.success_);
            std::swap(lhs.path_, rhs.path_);
        }

        /**
         * @brief Calculates the hash value of a FolderCreationResult object.
         * @param obj The FolderCreationResult object to hash.
         * @return The hash value of the object.
         */
        friend std::size_t hash_value(const FolderDeletionResult &obj) noexcept {
#ifdef __llvm__
            return bool_hasher(obj.success_) ^ (string_hasher(obj.path_.value_or("").string()) << 1);
#else
            return bool_hasher(obj.success_) ^ (path_hasher(obj.path_.value_or("")) << 1);
#endif
        }

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
                        LINFO("File '{}' deleted successfully.", filePath);
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
                        LINFO("Folder '{}' deleted successfully.", folderPath);
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
   // NOLINTEND(*-include-cleaner)