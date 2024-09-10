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
        // prettyPrint(*ast);
        LINFO("transpiled code: {}", transpileNode(*ast));
    }

    // Main code generation function
    std::string Transpiler::transpileNode(const vnd::ASTNode &node) {
        std::ostringstream code;

        // Determine the type of node and transpile code accordingly using helper functions
        if(const auto *binaryNode = node.safe_as<vnd::BinaryExpressionNode>()) {
            code << transpileBinaryExpressionNode(binaryNode);
        } else if(const auto *unaryNode = node.safe_as<vnd::UnaryExpressionNode>()) {
            code << transpileUnaryExpressionNode(unaryNode);
        } else if(const auto *variableNode = node.safe_as<vnd::VariableNode>()) {
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
        } else if(const auto *blliteralNode = node.safe_as<vnd::LiteralNode<bool>>()) {
            code << transpileLiteralNode(blliteralNode);
        } else if(const auto *chliteralNode = node.safe_as<vnd::LiteralNode<char>>()) {
            code << transpileLiteralNode(chliteralNode);
        } else if(const auto *svliteralNode = node.safe_as<vnd::LiteralNode<std::string_view>>()) {
            code << transpileLiteralNode(svliteralNode);
        } else if(const auto *typeNode = node.safe_as<vnd::TypeNode>()) {
            code << transpileTypeNode(typeNode);
        } else if(const auto *indexNode = node.safe_as<vnd::IndexNode>()) {
            code << transpileIndexNode(indexNode);
        } else if(const auto *arrayNode = node.safe_as<vnd::ArrayNode>()) {
            code << transpileArrayNode(arrayNode);
        } else {
            LERROR("Unknown or not handled node type: {}", node.getType());
        }

        return code.str();
    }

    // Helper function to transpile code for BinaryExpressionNode
    std::string Transpiler::transpileBinaryExpressionNode(const vnd::BinaryExpressionNode *binaryNode) {
        if(binaryNode == nullptr) { return ""; }
        std::ostringstream code;
        const auto op = binaryNode->getOp();
        if(op == ":") {
            code << transpileNode(*binaryNode->getRight());
            code << " ";
            code << transpileNode(*binaryNode->getLeft());
        } else {
            code << transpileNode(*binaryNode->getLeft());
            code << FORMAT(" {} ", op);
            code << transpileNode(*binaryNode->getRight());
        }
        return code.str();
    }

    // Helper function to transpile code for UnaryExpressionNode
    std::string Transpiler::transpileUnaryExpressionNode(const vnd::UnaryExpressionNode *unaryNode) {
        if(unaryNode == nullptr) { return ""; }
        return FORMAT("{}{}", unaryNode->getOp(), transpileNode(*unaryNode->getOperand()));
    }

    // Helper function to transpile code for VariableNode
    std::string Transpiler::transpileVariableNode(const vnd::VariableNode *variableNode) {
        if(variableNode == nullptr) { return ""; }
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
    template <typename T> std::string Transpiler::transpileNumericNode(const NumberNode<T> *numberNode) {
        if(!numberNode) { return ""; }
        return FORMAT("{}", numberNode->get_value());
    }

    // Helper function to transpile code for LiteralNode
    template <typename T> std::string Transpiler::transpileLiteralNode(const LiteralNode<T> *literalNode) {
        if(!literalNode) { return ""; }
        if constexpr(std::is_same_v<T, bool>) {
            return literalNode->get_value() ? "true" : "false";
        } else if constexpr(std::is_same_v<T, char>) {
            return FORMAT("'{}'", literalNode->get_value());
        } else if constexpr(std::is_same_v<T, std::string_view>) {
            return FORMAT("\"{}\"", literalNode->get_value());
        }
    }

    // Helper function to transpile code for TypeNode
    std::string Transpiler::transpileTypeNode(const vnd::TypeNode *typeNode) {
        if(typeNode == nullptr) { return ""; }
        std::ostringstream code;
        // code << typeNode.getName();
        code << typeNode->get_value();
        if(typeNode->get_index()) { code << FORMAT("{}", transpileNode(*typeNode->get_index())); }
        return code.str();
    }

    // Helper function to transpile code for IndexNode
    std::string Transpiler::transpileIndexNode(const vnd::IndexNode *indexNode) {
        if(indexNode == nullptr) { return ""; }
        std::ostringstream code;
        code << "[";
        if(const auto &elementsNode = indexNode->get_elements()) { code << FORMAT("{}", transpileNode(*elementsNode)); }
        code << "]";
        if(const auto &elementsIndexNode = indexNode->get_index()) { code << FORMAT("{}", transpileNode(*elementsIndexNode)); }
        if(const auto &elementsArrayNode = indexNode->get_array()) { code << FORMAT("{}", transpileNode(*elementsArrayNode)); }
        return code.str();
    }

    // Helper function to transpile code for ArrayNode
    std::string Transpiler::transpileArrayNode(const vnd::ArrayNode *arrayNode) {
        if(arrayNode == nullptr) { return ""; }
        std::ostringstream code;
        code << "{";
        if(arrayNode->get_elements()) { code << transpileNode(*arrayNode->get_elements()); }
        code << "}";
        return code.str();
    }

}  // namespace vnd

// clang-format off
// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-unused-variable, *-branch-clone, *-no-recursion, *-identifier-length)
// clang-format on
