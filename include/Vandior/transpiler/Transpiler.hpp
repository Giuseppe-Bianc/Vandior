#pragma once

#include "../FolderCreationResult.hpp"
#include "../parser/Parser.hpp"
#include "../timer/Timer.hpp"

namespace vnd {
    class Transpiler {
    public:
        Transpiler(const std::string_view &input, const std::string_view &filename);
        void transpile();

    private:
        std::string transpileNode(const ASTNode &node);
        std::string_view _filename;
        Parser _parser;
        fs::path _vnBuildSrcFolder;
        fs::path _mainOutputFilePath;
    };
}  // namespace vnd