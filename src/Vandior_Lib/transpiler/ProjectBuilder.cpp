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
            const auto projectname = _vnBuildFolder.value().parent_path().stem();
            outfile << FORMAT("# This is an automatically generated file by {}, do not modify.\n", generatorName);
            outfile << R"(# for more information got to  https://github.com/Giuseppe-Bianc/Vandior
cmake_minimum_required(VERSION 3.21)

)";
            outfile << FORMAT("project({0}s VERSION 1.0 LANGUAGES CXX C)\n\n", projectname);

            outfile << R"(set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.h" "*.cpp")
add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()
)";
            const auto result = vnd::FileCreationResult::createFile(_vnBuildFolder.value(), "CMakeLists.txt", outfile);

            return result.success();
        }
        return false;
    }

}  // namespace vnd

// NOLINTEND(*-include-cleaner)
