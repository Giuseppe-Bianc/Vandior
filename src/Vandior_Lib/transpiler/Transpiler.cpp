// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters, *-unused-variable, *-branch-clone, *-no-recursion)
#include "Vandior/transpiler/Transpiler.hpp"

namespace vnd {

    Transpiler::Transpiler(const std::string_view &input, const std::string_view &filename)
      : _filename(filename), _parser(input, _filename) {
        vnd::FolderCreationResult resultFolderCreationsrc;
        std::optional<fs::path> vnBuildFolder;
        std::optional<fs::path> vnSrcFolder;
        vnd::Timer folderTime("folder creation");
        auto resultFolderCreation = vnd::FolderCreationResult::createFolderNextToFile(filename.data(), "vnbuild");
        vnBuildFolder = resultFolderCreation.pathcref();
        if(!resultFolderCreation.success()) {
            return;
        } else {
            if(vnBuildFolder.has_value()) {
                resultFolderCreationsrc = vnd::FolderCreationResult::createFolder("src", vnBuildFolder.value());
            } else {
                return;
            }
            vnSrcFolder = resultFolderCreationsrc.pathcref();
        }
        LINFO("{}", folderTime);
        if(!resultFolderCreationsrc.success() && !vnSrcFolder.has_value()) { return; }
        if(vnSrcFolder.has_value()) {
            const vnd::AutoTimer timer("main file path creation");
            auto filecpp = fs::path(filename).replace_extension(".cpp").filename();
            _vnBuildSrcFolder = vnSrcFolder.value();
            _mainOutputFilePath = _vnBuildSrcFolder / filecpp;
        } else {
            return;
        }
        LINFO("build folder path {}", _vnBuildSrcFolder);
    }
    void Transpiler::transpile() {
        std::ofstream outfile(_mainOutputFilePath);

        // Verifica se il file è stato aperto correttamente
        if(!outfile) {
            LERROR("Errore nell'aprire il file {}", _mainOutputFilePath);
            return;
        }

        // Scrive del contenuto nel file
        outfile << "// Questo è un file generato automaticamente non modificare.\n";
        outfile << "#include <iostream>\n";
        outfile << "\n";
        outfile << "int main() {\n";
        outfile << "    std::cout << \"Hello, World!\" << std::endl;\n";
        outfile << "    return 0;\n";
        outfile << "}\n";

        // Chiude il file
        outfile.close();
        const auto ast = _parser.parse();
        prettyPrint(*ast);
        LINFO(transpileNode(*ast));
        LINFO("File {} creato con successo!", _mainOutputFilePath);
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
        std::ostringstream code;
        LINFO("transpileBinaryExpressionNode");
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

    /* std::string Transpiler::BinaryExpressionOrdered(const vnd::BinaryExpressionNode *binaryNode, bool leftFirst) {
        if(leftFirst) {
            return FORMAT("{} {} {}", transpileNode(*binaryNode->getLeft()), binaryNode->getOp(), transpileNode(*binaryNode->getRight()));
        } else {
            return FORMAT("{} {} {}", transpileNode(*binaryNode->getRight()), binaryNode->getOp(), transpileNode(*binaryNode->getLeft()));
        }
    }*/
    // Helper function to transpile code for UnaryExpressionNode
    std::string Transpiler::transpileUnaryExpressionNode(const vnd::UnaryExpressionNode *unaryNode) {
        return FORMAT("{}{}", unaryNode->getOp(), transpileNode(*unaryNode->getOperand()));
    }

    // Helper function to transpile code for VariableNode
    std::string Transpiler::transpileVariableNode(const vnd::VariableNode *variableNode) {
        std::ostringstream code;
        code << variableNode->getName();
        if(variableNode->is_call()) {
            code << "(";
            if(const auto &callNode = variableNode->get_call()) { code << transpileNode(*callNode); }
            code << ")";
        }
        if(const auto &indexNode = variableNode->get_index()) { code << FORMAT("[{}]", transpileNode(*indexNode)); }
        return code.str();
    }

    // Helper function to transpile code for Numeric nodes
    template <typename T> std::string Transpiler::transpileNumericNode(const NumberNode<T> *numberNode) {
        return FORMAT("{}", numberNode->get_value());
    }

    // Helper function to transpile code for LiteralNode
    template <typename T> std::string Transpiler::transpileLiteralNode(const LiteralNode<T> *literalNode) {
        if constexpr(std::is_same_v<T, bool>) {
            return literalNode->get_value() ? "true" : "false";
        } else if constexpr(std::is_same_v<T, char>) {
            return FORMAT("'{}'", literalNode->get_value());
        } else if constexpr(std::is_same_v<T, std::string_view>) {
            return FORMAT("\"{}\"", literalNode->get_value());
        }
        return "";
    }

    // Helper function to transpile code for TypeNode
    std::string Transpiler::transpileTypeNode(const vnd::TypeNode *typeNode) {
        std::ostringstream code;
        // code << typeNode.getName();
        code << typeNode->get_value();
        if(typeNode->get_index()) { code << FORMAT("[{}]", transpileNode(*typeNode->get_index())); }
        return code.str();
    }

    // Helper function to transpile code for IndexNode
    std::string Transpiler::transpileIndexNode(const vnd::IndexNode *indexNode) {
        std::ostringstream code;
        if(const auto &elementsNode = indexNode->get_elements()) { code << transpileNode(*elementsNode); }
        if(const auto &elementsIndexNode = indexNode->get_index()) { code << FORMAT("[{}]", transpileNode(*elementsIndexNode)); }
        if(const auto &elementsArrayNode = indexNode->get_array()) { code << FORMAT("{{{}}}", transpileNode(*elementsArrayNode)); }
        return code.str();
    }

    // Helper function to transpile code for ArrayNode
    std::string Transpiler::transpileArrayNode(const vnd::ArrayNode *arrayNode) {
        std::ostringstream code;
        code << "{ ";
        if(arrayNode->get_elements()) { code << transpileNode(*arrayNode->get_elements()); }
        code << "}";
        return code.str();
    }

}  // namespace vnd

// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-unused-variable, *-branch-clone, *-no-recursion)
