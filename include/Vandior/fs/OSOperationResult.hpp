// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "FSConstats.hpp"

DISABLE_WARNINGS_PUSH(4820)

namespace vnd {

    /**
     * @class OSOperationResult
     * @brief Represents the result of a folder operation (creation or deletion).
     */
    class OSOperationResult {
    public:
        /**
         * @brief Checks if the operation was successful.
         * @return True if the operation was successful, false otherwise.
         */
        [[nodiscard]] constexpr bool success() const noexcept { return success_; }

        /**
         * @brief Returns the path involved in the operation.
         * @return The path involved in the operation.
         */
        [[nodiscard]] std::optional<fs::path> path() const { return path_; }

        /**
         * @brief Returns the reference to the path involved in the operation.
         * @return The reference to the path involved in the operation.
         */
        [[nodiscard]] std::optional<fs::path> &pathref() noexcept { return path_; }

        /**
         * @brief Returns the const reference to the path involved in the operation.
         * @return The const reference to the path involved in the operation.
         */
        [[nodiscard]] const std::optional<fs::path> &pathcref() const noexcept { return path_; }

        /**
         * @brief Sets the success status of the operation.
         * @param success The success status of the operation.
         */
        void set_success(const bool success) noexcept { success_ = success; }

        /**
         * @brief Sets the path involved in the operation.
         * @param path The path involved in the operation.
         * @throws std::invalid_argument if the path is empty.
         */
        void set_path(const fs::path &path) {
            if(path.empty()) { throw std::invalid_argument("Path cannot be empty"); }
            path_ = path;
        }

        /**
         * @brief Default constructor.
         */
        /**
         * @brief Default constructor
         */
        OSOperationResult() = default;

        /**
         * @brief Constructs a FolderOperationResult with the specified success status and path.
         * @param success The success status of the operation.
         * @param path The path involved in the operation.
         */
        OSOperationResult(const bool success, const fs::path &path) : success_{success}, path_{path} {}

        /**
         * @brief Copy constructor for FolderOperationResult.
         */
        OSOperationResult(const OSOperationResult &other) = default;

        /**
         * @brief Move constructor for FolderOperationResult.
         */
        OSOperationResult(OSOperationResult &&other) noexcept = default;

        /**
         * @brief Copy assignment operator for FolderOperationResult.
         */
        OSOperationResult &operator=(const OSOperationResult &other) = default;

        /**
         * @brief Move assignment operator for FolderOperationResult.
         */
        OSOperationResult &operator=(OSOperationResult &&other) noexcept = default;

        /**
         * @brief Equality operator for FolderOperationResult.
         * @param lhs The left-hand side FolderOperationResult.
         * @param rhs The right-hand side FolderOperationResult.
         * @return True if both FolderOperationResult objects are equal, false otherwise.
         */
        friend bool operator==(const OSOperationResult &lhs, const OSOperationResult &rhs) noexcept = default;

        ~OSOperationResult() = default;

        friend void swap(OSOperationResult &lhs, OSOperationResult &rhs) noexcept {
            std::swap(lhs.success_, rhs.success_);
            std::swap(lhs.path_, rhs.path_);
        }

        /**
         * @brief Calculates the hash value of a FolderOperationResult object.
         * @param obj The FolderOperationResult object to hash.
         * @return The hash value of the object.
         */
        friend std::size_t hash_value(const OSOperationResult &obj) noexcept {
#ifdef __llvm__
            return bool_hasher(obj.success_) ^ (string_hasher(obj.path_.value_or("").string()) << 1);
#else
            return bool_hasher(obj.success_) ^ (path_hasher(obj.path_.value_or("")) << 1);
#endif
        }

        /**
         * @brief Outputs the FolderOperationResult object to an output stream.
         * @param oss The output stream.
         * @param obj The FolderOperationResult object to output.
         * @return The output stream after writing the FolderOperationResult object.
         */
        friend std::ostream &operator<<(std::ostream &oss, const OSOperationResult &obj) {
            const auto pathValue = obj.path_.value_or("");
            if(pathValue.empty()) { return oss << FORMAT("success_: {}, path_: None", obj.success_); }
            return oss << FORMAT("success_: {}, path_: {}", obj.success_, pathValue);
        }

    protected:
        bool success_ = false;          ///< The success status of the operation.
        std::optional<fs::path> path_;  ///< The path involved in the operation.
    };

}  // namespace vnd

DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner)