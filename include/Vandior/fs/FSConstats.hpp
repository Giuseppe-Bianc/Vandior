// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"

namespace vnd {
    constexpr std::hash<bool> bool_hasher;
#ifdef __llvm__
    constexpr std::hash<std::string> string_hasher;
#else
    constexpr std::hash<fs::path> path_hasher;
#endif
    inline auto GetBuildFolder(const fs::path &parentDir) -> fs::path { return parentDir.parent_path() / "vnbuild"; }
}  // namespace vnd

// NOLINTEND(*-include-cleaner)