#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Scene/Scene.h>
#include <dependencies/yaml-cpp/yaml.h>

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
        static bool OpenFileDialog(std::string &outFilePath, FileDialogType dialogType);
        static void OpenScene();
        static void SaveScene(Scene *scene);
    };
}