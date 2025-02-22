#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Core/Application.h>

namespace Azimuth
{
    class FileGenerator
    {
    public:
        static void GenerateScripts(std::filesystem::path path, const std::string &name);
        static void GenerateProjectFiles();

    private:
        static void GenerateDLLExportFiles();
        static void GenerateCMakeFile();
        static void UpdateDLLExportFile(std::filesystem::path path, const std::string &name);
    };
}