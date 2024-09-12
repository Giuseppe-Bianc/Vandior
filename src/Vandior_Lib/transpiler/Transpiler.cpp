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

    Transpiler::Transpiler(const std::string_view &input, const std::string_view &filename)
      : _filename(filename), _projectBuilder(filename), _parser(input, _filename) {
        _projectBuilder.buildProject();
        if(auto src_folderpo = _projectBuilder.getSrcFolderPath(); src_folderpo.has_value()) {
            _vnBuildSrcFolder = src_folderpo.value();
        } else {
            LERROR("Failed to get src folder path.");
        }
        if(auto mainOutputFilePathpo = _projectBuilder.getMainOutputFilePath(); mainOutputFilePathpo.has_value()) {
            _mainOutputFilePath = mainOutputFilePathpo.value();
        } else {
            LERROR("Failed to get main output file path");
        }
    }
    void Transpiler::createMockfile() {
        // Apre il file in modalità scrittura con RAII, nessuna necessità di close() manuale
        std::ofstream outfile(_mainOutputFilePath);

        // Verifica se il file è stato aperto correttamente
        if(!outfile.is_open()) {
            LERROR("Errore nell'aprire il file {}", _mainOutputFilePath);
            return;
        }

        auto generatorName = GENERATOR_FULLNAME;

        outfile << FORMAT("// This is an automatically generated file by {}, do not modify.", generatorName);
        outfile << fileContent;

        // Il file viene chiuso automaticamente qui

#ifdef INDEPT
        // Usa std::format per la formattazione delle stringhe (C++20)
        LINFO("File {} creato con successo!", _mainOutputFilePath);
#endif  // INDEPT
    }
    void Transpiler::transpile() {
        createMockfile();
        const auto ast = _parser.parse();
        const auto transpiledCode = transpileNode(*ast);
        LINFO("transpiled code: {}", transpiledCode);
    }

    // Main code generation function
    auto Transpiler::transpileNode(const ASTNode &node) -> std::string {
        std::ostringstream code;

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
        } else if(const auto *typeNode = node.safe_as<TypeNode>()) {
            code << transpileTypeNode(typeNode);
        } else if(const auto *indexNode = node.safe_as<IndexNode>()) {
            code << transpileIndexNode(indexNode);
        } else if(const auto *arrayNode = node.safe_as<ArrayNode>()) {
            code << transpileArrayNode(arrayNode);
        } else {
            LERROR("Unknown or not handled node type: {}", node.getType());
        }

        return code.str();
    }

    void Transpiler::transpileBinaryExpressionNodeWhitFormat(const BinaryExpressionNode *binaryNode, std::ostringstream &code,
                                                             const std::string &format) {
        code << transpileNode(*binaryNode->getLeft());
        code << format;
        code << transpileNode(*binaryNode->getRight());
    }
    // Helper function to transpile code for BinaryExpressionNode
    auto Transpiler::transpileBinaryExpressionNode(const BinaryExpressionNode *binaryNode) -> std::string {
        if(binaryNode == nullptr) { return ""; }
        std::ostringstream code;
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
        std::ostringstream code;
        code << variableNode->getName();
        if(variableNode->is_call()) {
            code << "(";
            if(const auto &callNode = variableNode->get_call()) { code << transpileNode(*callNode); }
            code << ")";
        }
        if(const auto &indexNode = variableNode->get_index()) { code << FORMAT("{}", transpileNode(*indexNode)); }
        return code.str();
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
            {"i32"sv, "int"sv},
            {"i64"sv, "double"sv},
            {"f32"sv, "float"sv},
            {"f64"sv, "double"sv},
            {"c32"sv, "std::complex<float>"sv},
            {"c64"sv, "std::complex<double>"sv},
            {"bool"sv, "bool"sv},
            {"char"sv, "char"sv},
            {"string"sv, "std::string_view"sv},
        };

        if(typeMap.contains(type)) { return typeMap.at(type); }
        return "unknown"sv;  // Default case if type is not found
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
            std::ostringstream code;
            code << mappedType;
            if(typeNode->get_index()) { code << FORMAT("{}", transpileNode(*typeNode->get_index())); }
            return code.str();
        }
    }

    // Helper function to transpile code for IndexNode
    auto Transpiler::transpileIndexNode(const IndexNode *indexNode) -> std::string {
        if(indexNode == nullptr) [[unlikely]] { return ""; }
        std::ostringstream code;
        code << "[";
        if(const auto &elementsNode = indexNode->get_elements()) { code << FORMAT("{}", transpileNode(*elementsNode)); }
        code << "]";
        if(const auto &elementsIndexNode = indexNode->get_index()) { code << FORMAT("{}", transpileNode(*elementsIndexNode)); }
        if(const auto &elementsArrayNode = indexNode->get_array()) { code << FORMAT("{}", transpileNode(*elementsArrayNode)); }
        return code.str();
    }

    // Helper function to transpile code for ArrayNode
    auto Transpiler::transpileArrayNode(const ArrayNode *arrayNode) -> std::string {
        if(arrayNode == nullptr) [[unlikely]] { return ""; }
        std::ostringstream code;
        code << "{";
        if(const auto &elementsNode = arrayNode->get_elements()) { code << transpileNode(*elementsNode); }
        code << "}";
        return code.str();
    }

}  // namespace vnd

// clang-format off
// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-unused-variable, *-branch-clone, *-no-recursion, *-identifier-length)
// clang-format on
