/*
 * Created by gbian on 09/09/2024.
 * Copyright (c) 2024 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "Vandior/transpiler/ProjectBuilder.hpp"
namespace vnd {
    // Costruttore che prende il nome del file come parametro
    ProjectBuilder::ProjectBuilder(const std::string_view &filename, bool createCmakeListsFile) noexcept
      : _createCmakeListsFile(createCmakeListsFile), _filename(filename) {}

    // Metodo pubblico per avviare la costruzione del progetto
    void ProjectBuilder::buildProject() {
#ifdef INDEPT
        vnd::Timer folderTime("folder creation");
#endif

        if(!createBuildFolder()) {
            LERROR("Failed to create build folder.");
            return;
        }

        if(!createSrcFolder()) {
            LERROR("Failed to create src folder.");
            return;
        }
#ifdef INDEPT
        LINFO("{}", folderTime);
#endif
        if(_createCmakeListsFile) {
            if(!createCMakeListsFile()) { LERROR("Failed to create CMakeLists.txt file."); }
        }

        if(!createMainFile()) {
            LERROR("Failed to create main file.");
            return;
        }

#ifdef INDEPT
        LINFO("Build folder path {}", _vnSrcFolder.value());  // NOLINT(*-unchecked-optional-access)
#endif
    }

    // Getter per ottenere il percorso della cartella 'vnbuild'
    std::optional<fs::path> ProjectBuilder::getBuildFolderPath() const { return _vnBuildFolder; }

    // Getter per ottenere il percorso della cartella 'src'
    std::optional<fs::path> ProjectBuilder::getSrcFolderPath() const { return _vnSrcFolder; }

    // Getter per ottenere il percorso del file principale .cpp
    std::optional<fs::path> ProjectBuilder::getMainOutputFilePath() const { return _mainOutputFilePath; }

    // Metodo per creare la cartella 'vnbuild'
    bool ProjectBuilder::createBuildFolder() {
        const auto resultFolderCreation = vnd::FolderCreationResult::createFolderNextToFile(_filename.data(), "vnbuild");
        _vnBuildFolder = resultFolderCreation.pathcref();

        if(!resultFolderCreation.success()) { return false; }
        return _vnBuildFolder.has_value();
    }

    // Metodo per creare la cartella 'src' all'interno della cartella 'vnbuild'
    bool ProjectBuilder::createSrcFolder() {
        if(_vnBuildFolder.has_value()) {
            const auto resultFolderCreationsrc = vnd::FolderCreationResult::createFolder("src", _vnBuildFolder.value());
            _vnSrcFolder = resultFolderCreationsrc.pathcref();

            return resultFolderCreationsrc.success() && _vnSrcFolder.has_value();
        }
        return false;
    }

    // Metodo per creare il file principale .cpp all'interno della cartella 'src'
    bool ProjectBuilder::createMainFile() {
        if(_vnSrcFolder.has_value()) {
#ifdef INDEPT
            const vnd::AutoTimer timer("main file path creation");
#endif
            const auto filecpp = fs::path(_filename).replace_extension(".cpp").filename();
            _mainOutputFilePath = _vnSrcFolder.value() / filecpp;
            return true;
        }
        return false;
    }

    bool ProjectBuilder::createCMakeListsFile() {
        if(_vnBuildFolder.has_value()) {
#ifdef INDEPT
            const vnd::AutoTimer timer("CMakeLists.txt file creation");
#endif
            std::stringstream outfile;

            const auto generatorName = GENERATOR_FULLNAME;
            outfile << FORMAT("# This is an automatically generated file by {}, do not modify. for more information got to  "
                              "https://github.com/Giuseppe-Bianc/Vandior\n",
                              generatorName);

            outfile << "cmake_minimum_required(VERSION 3.0)\n\n";
            const auto projectname = _vnBuildFolder.value().parent_path().stem();
            outfile << FORMAT("project({}s VERSION 1.0 LANGUAGES CXX C)\n\n", projectname);
            outfile << "set(CMAKE_CXX_STANDARD 20)\n";
            outfile << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n";
            outfile << "set(CMAKE_CXX_EXTENSIONS OFF)\n\n";

            outfile << "file(GLOB_RECURSE sources      src/*.cpp src/*.h)\n";
            outfile << "add_executable(${PROJECT_NAME} ${SOURCES})\n\n";

            outfile << "set_target_properties(${PROJECT_NAME} PROPERTIES\n";
            outfile << "    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin\n";
            outfile << "    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib\n";
            outfile << "    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib\n";
            outfile << ")\n\n";

            outfile << "if (CMAKE_CXX_COMPILER_ID STREQUAL \\\"GNU\\\" OR CMAKE_CXX_COMPILER_ID STREQUAL \\\"Clang\\\")\n";
            outfile << "    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)\n";
            outfile << "elseif (CMAKE_CXX_COMPILER_ID STREQUAL \\\"MSVC\\\")\n";
            outfile << "    target_compile_options(${PROJECT_NAME} PRIVATE /W4)\n";
            outfile << "endif()\n";
            const auto result = vnd::FileCreationResult::createFile(_vnBuildFolder.value(), "CMakeLists.txt", outfile);

            return result.success();
        }
        return false;
    }

}  // namespace vnd

// NOLINTEND(*-include-cleaner)
