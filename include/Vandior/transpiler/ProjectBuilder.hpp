// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../fs/fs.hpp"
#include "../headers.hpp"

namespace vnd {

    class ProjectBuilder {
    public:
        // Costruttore che prende il nome del file come parametro
        explicit ProjectBuilder(const std::string_view &filename, const bool createCmakeListsFile = false) noexcept;
        // Metodo pubblico per avviare la costruzione del progetto
        void buildProject();

        // Getter per ottenere il percorso della cartella 'vnbuild'
        [[nodiscard]] std::optional<fs::path> getBuildFolderPath() const;

        // Getter per ottenere il percorso della cartella 'src'
        [[nodiscard]] std::optional<fs::path> getSrcFolderPath() const;

        // Getter per ottenere il percorso del file principale .cpp
        [[nodiscard]] std::optional<fs::path> getMainOutputFilePath() const;

    private:
        bool _createCmakeListsFile;
        std::string_view _filename;                   // Nome del file di input
        std::optional<fs::path> _vnBuildFolder;       // Percorso della cartella 'vnbuild'
        std::optional<fs::path> _vnSrcFolder;         // Percorso della cartella 'src'
        std::optional<fs::path> _mainOutputFilePath;  // Percorso del file principale .cpp

        // Metodo per creare la cartella 'vnbuild'
        [[nodiscard]] bool createBuildFolder();

        // Metodo per creare la cartella 'src' all'interno della cartella 'vnbuild'
        [[nodiscard]] bool createSrcFolder();

        // Metodo per creare il file principale .cpp all'interno della cartella 'src'
        [[nodiscard]] bool createMainFile();

        [[nodiscard]] bool createCMakeListsFile() const;
    };

}  // namespace vnd

// NOLINTEND(*-include-cleaner)
