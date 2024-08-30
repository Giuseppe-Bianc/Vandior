// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters)
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
    void Transpiler::transpile() const {
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

        LINFO("File {} creato con successo!", _mainOutputFilePath);
    }

}  // namespace vnd

// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters)