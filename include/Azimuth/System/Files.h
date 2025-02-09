#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
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
        static std::vector<std::filesystem::path> GetFilesWithExtension(const std::filesystem::path &directory, const std::string &extension);
    };
}