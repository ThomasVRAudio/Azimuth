#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    class Scene;

    class Serializer
    {
    public:
        static void OpenProject(const std::string &path = "", Scene *scene = nullptr);
        static void SaveProject();
        static void OpenScene(Scene *scene, std::string path = "", std::string *outPath = nullptr);
        static void SaveScene(Scene *scene);
    };
}