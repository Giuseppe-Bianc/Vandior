#include "Vandior/transpiler/Transpiler.hpp"

namespace vnd {

    Transpiler::Transpiler(const std::string_view &input, const std::string_view &filename): _filename(filename), _parser(input, _filename) {
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
        if(!resultFolderCreationsrc.success() && !vnSrcFolder.has_value()) {
            return;
        } else {
            LINFO("build folder path {}", vnSrcFolder.value());
        }
    }
    void Transpiler::transpile() const {

    }

}