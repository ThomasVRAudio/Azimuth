#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Scene/Scene.h>
#include <dependencies/yaml-cpp/yaml.h>
#include <Azimuth/Project/YamlConversions.h>

namespace Azimuth
{
    enum FileDialogType
    {
        OPEN,
        SAVE
    };

    class Serializer
    {
    public:
        static bool OpenFileDialog(std::string &outFilePath, FileDialogType dialogType, const char *fileFilter);
        static void OpenProject(const std::string &path = "", Scene *scene = nullptr);
        static void SaveProject();
        static void OpenScene(Scene *scene, std::string path = "", std::string *outPath = nullptr);
        static void SaveScene(Scene *scene);

    private:
        static void ClearScene(Scene *scene);
    };
}