// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters, *-unused-variable, *-branch-clone, *-no-recursion,*-identifier-length)
// clang-format on
#include "Vandior/transpiler/Transpiler.hpp"
using namespace std::string_view_literals;

static inline constexpr auto fileContent = R"(
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
)"sv;
namespace vnd {
    static inline constexpr std::size_t INITIAL_BUFFER_SIZE = 100;
    static inline constexpr std::size_t INITIAL_TOTAL_BUFFER_SIZE = 1024;

    // Simplify handling of std::optional with fallback logging function
    template <typename T> auto Transpiler::getValueOrLog(const std::optional<T> &opt, std::string_view errorMsg) -> T {
        if(opt.has_value()) { return opt.value(); }
        LERROR(errorMsg);
        return {};  // Default-constructed value
    }

    Transpiler::Transpiler(const std::string_view &input, const std::string_view &filename)
      : _filename(filename), _projectBuilder(filename), _parser(input, _filename) {
        _projectBuilder.buildProject();
        _vnBuildFolder = getValueOrLog(_projectBuilder.getBuildFolderPath(), "Failed to get build folder path.");
        _vnBuildSrcFolder = getValueOrLog(_projectBuilder.getSrcFolderPath(), "Failed to get src folder path.");
        _mainOutputFilePath = getValueOrLog(_projectBuilder.getMainOutputFilePath(), "Failed to get main output file path.");
    }
    void Transpiler::createMockfile() {
        // Apre il file in modalità scrittura con RAII, nessuna necessità di close() manuale
        std::ofstream outfile(_mainOutputFilePath);

        // Verifica se il file è stato aperto correttamente
        if(!outfile.is_open()) {
            LERROR("Errore nell'aprire il file {}", _mainOutputFilePath);
            return;
        }

        const auto generatorName = GENERATOR_FULLNAME;

        outfile << FORMAT("// This is an automatically generated file by {}, do not modify.", generatorName);
        outfile << fileContent;

        // Il file viene chiuso automaticamente qui

#ifdef INDEPT
        // Usa std::format per la formattazione delle stringhe (C++20)
        LINFO("File {} creato con successo!", _mainOutputFilePath);
#endif  // INDEPT
    }
    std::string Transpiler::transpile() {
        createMockfile();
        std::stringstream out;
        using enum TokenType;
        for(const auto ast = _parser.parse(); const auto &i : ast) {
            const auto &node = i.get_nodes().at(0);
            out << transpileKeyword(i.get_token());
            if(node) {
                auto code = transpileNode(*node);
                if(i.get_token().getType() == TokenType::K_VAR) { code = parseDeclaration(code);
                    LWARN("{}", code);
                }
                out << code;
            }
            const auto stTknType = i.get_token().getType();
            if(checkKeyword(stTknType).second) {
                if(stTknType != K_FUN && stTknType != K_MAIN) {
                    out << ")";
                } else if(stTknType == K_FUN) {
                    out << " ->";
                    const auto &data = i.get_funData();
                    if(data.empty()) {
                        out << " void";
                    } else if(data.size() == 1) {
                        out << FORMAT(" {}", mapType(data.front()));
                    } else {
                        out << " std::tuple<";
                        for(const auto &j : i.get_funData()) { out << FORMAT(" {}", mapType(j)); }
                        out << ">";
                    }
                }
                out << " {";
            }
            out << "\n";
        }
        return out.str();
    }

    // NOLINTBEGIN(*-convert-member-functions-to-static)
    auto Transpiler::transpileKeyword(const Token &keyword) -> std::string {
        using enum TokenType;

        switch(keyword.getType()) {
        case K_MAIN:
            return "int main(int argc, char **argv)";
        case K_VAR:
            return keyword.getValue() == "const" ? "const " : "";
        case K_IF:
            return "if(";
        case K_WHILE:
            return "while(";
        case K_FOR:
            return "for(";
        case K_BREAK:
            return std::string{keyword.getValue()};
        case K_FUN:
            return "auto ";
        case K_RETURN:
            return "return ";
        default:
            return "";
        }
    }
    // NOLINTEND(*-convert-member-functions-to-static)

    std::string Transpiler::parseDeclaration(std::string input) {
        std::ostringstream out;
        std::vector<std::string> identifiers;
        std::vector<std::string> values;
        std::vector<std::string> *current = &identifiers;
        std::string currentToken;
        bool isArray = false;
        bool isType = false;
        size_t start = input.find_first_of(' ');
        out << input.substr(0, start);
        input = input.substr(start);
        LWARN(input);
        for(const auto i : input) {
            if(i == ',' && !isArray && !isType) {
                current->emplace_back(currentToken);
                currentToken.clear();
            } else if(i == '=') {
                current->emplace_back(currentToken);
                currentToken.clear();
                current = &values;
            } else {
                if(i == '{' && !isType) {
                    isArray = true;
                } else if(i == '}' && isArray) {
                    isArray = false;
                } else if(i == '<' && !isArray) {
                    isType = true;
                } else if(i == '>' and isType) {
                    isType = false;
                }
                currentToken += i;
            }
        }
        if(!currentToken.empty()) {
            current->emplace_back(currentToken);
        }
        auto it = values.begin();
        bool first = true;
        for(const auto &i : identifiers) {
            if(!first) {
                out << ",";
            } else {
                first = false;
            }
            out << i;
            if(it != values.end()) {
                out << FORMAT(" ={}", *it);
                ++it;
            }
        }
        return out.str();
    }

    // Main code generation function
    auto Transpiler::transpileNode(const ASTNode &node) -> std::string {
        std::ostringstream code;
        code.str().reserve(INITIAL_TOTAL_BUFFER_SIZE);

        // Determine the type of node and transpile code accordingly using helper functions
        if(const auto *binaryNode = node.safe_as<BinaryExpressionNode>()) {
            code << transpileBinaryExpressionNode(binaryNode);
        } else if(const auto *unaryNode = node.safe_as<UnaryExpressionNode>()) {
            code << transpileUnaryExpressionNode(unaryNode);
        } else if(const auto *variableNode = node.safe_as<VariableNode>()) {
            code << transpileVariableNode(variableNode);
        } else if(const auto *intnumberNode = node.safe_as<VND_NUM_INT>()) {
            code << transpileNumericNode(intnumberNode);
        } else if(const auto *cflnumberNode = node.safe_as<VND_NUM_CFLOAT>()) {
            code << transpileNumericNode(cflnumberNode);
        } else if(const auto *dflnumberNode = node.safe_as<VND_NUM_CDOUBLE>()) {
            code << transpileNumericNode(dflnumberNode);
        } else if(const auto *flnumberNode = node.safe_as<VND_NUM_FLOAT>()) {
            code << transpileNumericNode(flnumberNode);
        } else if(const auto *dblnumberNode = node.safe_as<VND_NUM_DOUBLE>()) {
            code << transpileNumericNode(dblnumberNode);
        } else if(const auto *blliteralNode = node.safe_as<LiteralNode<bool>>()) {
            code << transpileLiteralNode(blliteralNode);
        } else if(const auto *chliteralNode = node.safe_as<LiteralNode<char>>()) {
            code << transpileLiteralNode(chliteralNode);
        } else if(const auto *svliteralNode = node.safe_as<LiteralNode<std::string_view>>()) {
            code << transpileLiteralNode(svliteralNode);
        } else if([[maybe_unused]] const auto *nullptrNode = node.safe_as<NullptrNode>()) {
            code << "nullptr";
        } else if(const auto *typeNode = node.safe_as<TypeNode>()) {
            code << transpileTypeNode(typeNode);
        } else if(const auto *arrayNode = node.safe_as<ArrayNode>()) {
            code << transpileArrayNode(arrayNode);
        } else {
            LERROR("Unknown or not handled node type: {}", node.getType());
        }

        return code.str();
    }

    DISABLE_WARNINGS_PUSH(26429)
    void Transpiler::transpileBinaryExpressionNodeWhitFormat(const BinaryExpressionNode *binaryNode, std::ostringstream &code,
                                                             const std::string &format) {
        code << transpileNode(*binaryNode->getLeft());
        code << format;
        code << transpileNode(*binaryNode->getRight());
    }
    DISABLE_WARNINGS_POP()
    // Helper function to transpile code for BinaryExpressionNode
    auto Transpiler::transpileBinaryExpressionNode(const BinaryExpressionNode *binaryNode) -> std::string {
        if(binaryNode == nullptr) { return ""; }
        std::ostringstream code;
        code.str().reserve(INITIAL_BUFFER_SIZE);
        const auto op = binaryNode->getOp();
        if(op == ":") {
            const auto *binaryRight = binaryNode->getRight()->safe_as<BinaryExpressionNode>();
            if(binaryRight != nullptr) [[likely]] {
                code << transpileNode(*binaryRight->getLeft());
            } else [[unlikely]] {
                code << transpileNode(*binaryNode->getRight());
            }
            code << FORMAT(" {}", transpileNode(*binaryNode->getLeft()));
            if(binaryRight != nullptr) [[likely]] {
                code << FORMAT(" {} ", binaryRight->getOp());
                code << transpileNode(*binaryRight->getRight());
            }
        } else if(op == ",") {
            transpileBinaryExpressionNodeWhitFormat(binaryNode, code, FORMAT("{} ", op));
        } else {
            transpileBinaryExpressionNodeWhitFormat(binaryNode, code, FORMAT(" {} ", op));
        }
        return code.str();
    }

    // Helper function to transpile code for UnaryExpressionNode
    auto Transpiler::transpileUnaryExpressionNode(const UnaryExpressionNode *unaryNode) -> std::string {
        if(unaryNode == nullptr) [[unlikely]] { return ""; }
        return FORMAT("{}{}", unaryNode->getOp(), transpileNode(*unaryNode->getOperand()));
    }

    // Helper function to transpile code for VariableNode
    auto Transpiler::transpileVariableNode(const VariableNode *variableNode) -> std::string {
        if(variableNode == nullptr) [[unlikely]] { return ""; }
        TranspileContext context;
        if(const auto &indexNode = variableNode->get_index()) {
            context.setIndexAndArray(transpileIndexNode(indexNode.get()));
        } else {
            context.index = "{}";
        }
        context.code << fmt::vformat(context.index, fmt::make_format_args(variableNode->getName()));
        if(!context.arr.empty()) { context.code << FORMAT("({})", context.arr); }
        if(variableNode->is_call()) {
            context.code << "(";
            if(const auto &callNode = variableNode->get_call()) { context.code << transpileNode(*callNode); }
            context.code << ")";
        }
        return context.code.str();
    }

    // Helper function to transpile code for Numeric nodes
    template <typename T> auto Transpiler::transpileNumericNode(const NumberNode<T> *numberNode) -> std::string {
        if(numberNode == nullptr) [[unlikely]] { return ""; }
        return FORMAT("{}", numberNode->get_value());
    }

    // Helper function to transpile code for LiteralNode
    template <typename T> auto Transpiler::transpileLiteralNode(const LiteralNode<T> *literalNode) -> std::string {
        if(literalNode == nullptr) [[unlikely]] { return ""; }
        if constexpr(std::is_same_v<T, bool>) {
            return literalNode->get_value() ? "true" : "false";
        } else if constexpr(std::is_same_v<T, char>) {
            return FORMAT("'{}'", literalNode->get_value());
        } else if constexpr(std::is_same_v<T, std::string_view>) {
            return FORMAT("\"{}\"", literalNode->get_value());
        } else {
            return "";
        }
    }

    auto Transpiler::mapType(const std::string_view type) -> std::string_view {
        static const std::unordered_map<std::string_view, std::string_view> typeMap = {
            {"i8"sv, "int8_t"sv},
            {"i16"sv, "int16_t"sv},
            {"i32"sv, "int32_t"sv},
            {"i64"sv, "int64_t"sv},
            {"u8"sv, "uint8_t"sv},
            {"u16"sv, "uint16_t"sv},
            {"u32"sv, "uint32_t"sv},
            {"u64"sv, "uint64_t"sv},
            {"f32"sv, "float"sv},
            {"f64"sv, "double"sv},
            {"c32"sv, "std::complex<float>"sv},
            {"c64"sv, "std::complex<double>"sv},
            {"bool"sv, "bool"sv},
            {"char"sv, "char"sv},
            {"string"sv, "std::string_view"sv},
        };

        if(typeMap.contains(type)) { return typeMap.at(type); }
        return type;  // Default case if type is not found
    }
    // Helper function to transpile code for TypeNode
    auto Transpiler::transpileTypeNode(const TypeNode *typeNode) -> std::string {
        if(typeNode == nullptr) [[unlikely]] { return ""; }
        // code << typeNode.getName();
        const auto initaltype = typeNode->get_value();
        const auto mappedType = mapType(initaltype);
        if(mappedType == "unknown"sv) [[unlikely]] {
            return std::string(initaltype);
        } else [[likely]] {
            TranspileContext context;
            if(const auto &indexNode = typeNode->get_index()) {
                context.setIndexAndArray(transpileIndexNode(indexNode.get()));
            } else {
                context.index = "{}";
            }
            context.code << fmt::vformat(context.index, fmt::make_format_args(mappedType));
            if(!context.arr.empty()) { context.code << FORMAT("({})", context.arr); }
            return context.code.str();
        }
    }

    // Helper function to transpile code for IndexNode
    auto Transpiler::transpileIndexNode(const IndexNode *indexNode) -> std::pair<std::string, std::string> {
        if(indexNode == nullptr) [[unlikely]] { return {}; }
        std::string type;
        TranspileContext context;
        if(const auto &elementsIndexNode = indexNode->get_index()) {
            context.setIndexAndArray(transpileIndexNode(elementsIndexNode.get()));
        } else {
            context.index = "{}";
        }
        if(const auto &elements = indexNode->get_elements()) {
            type = FORMAT("vnd::array<{{}}, {}>", transpileNode(*elements));
        } else {
            type = "vnd::vector<{}>";
        }
        context.code << fmt::vformat(type, fmt::make_format_args(context.index));
        if(!context.arr.empty()) { return std::make_pair(context.code.str(), context.arr); }
        if(const auto &elementsArrayNode = indexNode->get_array()) {
            return std::make_pair(context.code.str(), FORMAT("{}", transpileNode(*elementsArrayNode)));
        }
        return std::make_pair(context.code.str(), "");
    }

    // Helper function to transpile code for ArrayNode
    auto Transpiler::transpileArrayNode(const ArrayNode *arrayNode) -> std::string {
        if(arrayNode == nullptr) [[unlikely]] { return ""; }
        std::ostringstream code;
        code.str().reserve(INITIAL_BUFFER_SIZE);
        code << "{";
        if(const auto &elementsNode = arrayNode->get_elements()) { code << transpileNode(*elementsNode); }
        code << "}";
        return code.str();
    }

}  // namespace vnd

// clang-format off
// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-unused-variable, *-branch-clone, *-no-recursion, *-identifier-length)
// clang-format on
