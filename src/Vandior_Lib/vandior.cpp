// NOLINTBEGIN(*-include-cleaner)
#include "Vandior/vandior.hpp"

namespace vnd {

    auto timeParser(std::vector<Statement> &ast, Parser &parser) -> void {
#ifdef INDEPT
        const AutoTimer timer("parse");
#endif
        ast = vnd_move_always_even_const(parser.parse());
    }

    auto timeParse(Parser &parser) -> std::vector<Statement> {
        std::vector<Statement> ast;
        timeParser(ast, parser);
        return ast;
    }
}  // namespace vnd

// NOLINTEND(*-include-cleaner)