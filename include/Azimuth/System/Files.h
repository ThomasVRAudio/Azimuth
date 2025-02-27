#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Application.h>
#include <shobjidl.h>

namespace Azimuth
{

    enum class FileDialogType
    {
        OPEN,
        SAVE
    };

    enum class FileType
    {
        Directory,
        File
    };

    class Files
    {
    public:
        static std::vector<std::pair<FileType, std::string>> GetFilenamesFromDirectory(const std::string &directory);
        static bool GetShaderInfoFromFile(const std::string &filepath, std::string &vertPath, bool &lit);
        static std::vector<std::filesystem::path> GetFilesWithExtension(const std::filesystem::path &directory, const std::vector<std::string> &extension);
        static bool OpenFolderDialog(std::string &outFilePath);
        static bool OpenFileDialog(std::string &outFilePath, FileDialogType dialogType, const char *fileFilter);
    };
}