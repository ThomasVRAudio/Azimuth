#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    struct ProjectSettings
    {
        std::filesystem::path ProjectFolder;
        std::filesystem::path MainScenePath;

        ProjectSettings(const std::filesystem::path &folder) : ProjectFolder(folder) {}
    };
}