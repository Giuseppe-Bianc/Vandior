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
    inline auto GetBuildFolder(const fs::path &parentDir) -> fs::path {
        fs::path parent = parentDir.lexically_normal();
        if(parent.filename() == "") { parent = parent.parent_path(); }
        if(parent == "..") { return parent / VANDIOR_BUILDFOLDER; }
        return parent.parent_path() / VANDIOR_BUILDFOLDER;
    }

}  // namespace vnd

// NOLINTEND(*-include-cleaner)