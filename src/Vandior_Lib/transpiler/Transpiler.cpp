// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters, *-unused-variable, *-branch-clone)
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

        LINFO("File {} creato con successo!", _mainOutputFilePath);
    }

    std::string Transpiler::transpileNode(const ASTNode &node) {
        std::ostringstream code;
        // Determine the type of node and print information
        if([[maybe_unused]] const auto *binaryNode = node.safe_as<vnd::BinaryExpressionNode>()) {
            // prettyPrint(*binaryNode->getLeft(), newindent, false, "LEFT");
            // prettyPrint(*binaryNode->getRight(), newindent, true, "RIGHT");
        } else if([[maybe_unused]] const auto *unaryNode = node.safe_as<vnd::UnaryExpressionNode>()) {
            // prettyPrint(*unaryNode->getOperand(), newindent, true, "OPERAND");
        } else if([[maybe_unused]] const auto *variableNode = node.safe_as<vnd::VariableNode>()) {
            // if(variableNode->is_call()) {
            //     LINFO("{}()", indentmark);
            //     if(const auto &callNode = variableNode->get_call()) { prettyPrint(*callNode, newindent, true, "CALL"); }
            // }
            // if(const auto &indexNode = variableNode->get_index()) { prettyPrint(*indexNode, newindent, true, "INDEX"); }
        } else if([[maybe_unused]] const auto *intnumberNode = node.safe_as<VND_NUM_INT>()) {
        } else if([[maybe_unused]] const auto *cflnumberNode = node.safe_as<VND_NUM_CFLOAT>()) {
        } else if([[maybe_unused]] const auto *dflnumberNode = node.safe_as<VND_NUM_CDOUBLE>()) {
        } else if([[maybe_unused]] const auto *flnumberNode = node.safe_as<VND_NUM_FLOAT>()) {
        } else if([[maybe_unused]] const auto *dblnumberNode = node.safe_as<VND_NUM_DOUBLE>()) {
        } else if([[maybe_unused]] const auto *blliteralNode = node.safe_as<vnd::LiteralNode<bool>>()) {
        } else if([[maybe_unused]] const auto *chliteralNode = node.safe_as<vnd::LiteralNode<char>>()) {
        } else if([[maybe_unused]] const auto *svliteralNode = node.safe_as<vnd::LiteralNode<std::string_view>>()) {
        } else if([[maybe_unused]] const auto *typeNode = node.safe_as<vnd::TypeNode>()) {
            // if(typeNode->get_index()) { prettyPrint(*typeNode->get_index(), newindent, true, "INDEX"); }
        } else if(const auto *indexNode = node.safe_as<vnd::IndexNode>()) {
            [[maybe_unused]] const auto &elementsNode = indexNode->get_elements();
            [[maybe_unused]] const auto &elementsIndexNode = indexNode->get_index();
            [[maybe_unused]] const auto &elementsArrayNode = indexNode->get_array();
            // if(elementsNode) { prettyPrint(*elementsNode, newindent, true, ""); }
            // if(elementsIndexNode) { prettyPrint(*elementsIndexNode, newindent, true, "INDEX"); }
            // if(elementsArrayNode) { prettyPrint(*elementsArrayNode, newindent, true, "ELEM"); }
        } else if([[maybe_unused]] const auto *arrayNode = node.safe_as<vnd::ArrayNode>()) {
            // if(arrayNode->get_elements()) { prettyPrint(*arrayNode->get_elements(), newindent, true, ""); }
        } else {
            LERROR("Unknown or not handled node type: {}", node.getType());
        }

        return code.str();
    }

}  // namespace vnd

// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-unused-variable, *-branch-clone)