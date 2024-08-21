#pragma once

#include "../parser/Parser.hpp"
#include "../FolderCreationResult.hpp"
#include "../timer/Timer.hpp"

namespace vnd {
    class Transpiler {

        public:
            Transpiler(const std::string_view &input, const std::string_view &filename);
            void transpile() const;
        private:
            std::string_view _filename;
            Parser _parser;

    };
}