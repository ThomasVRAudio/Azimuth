#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Core/Application.h>

namespace Azimuth
{
    class FileGenerator
    {
    public:
        static void GenerateScripts(std::filesystem::path path, const std::string &name);
        static void GenerateGLSLFile(const std::filesystem::path &path, const std::string &name);
        static void GenerateProjectFiles(const std::filesystem::path &filePath, Scene *scene);

    private:
        static void GenerateDLLExportFiles(const std::filesystem::path &folderPath);
        static void GenerateCMakeFile(const std::filesystem::path &folderPath);
        static void GenerateSceneFile(const std::filesystem::path &folderPath);
        static void GenerateProjectSettingsFile(const std::filesystem::path &fileName);
        static void UpdateDLLExportFile(const std::filesystem::path &path, const std::string &name, const std::filesystem::path &projectFolder);
    };
}