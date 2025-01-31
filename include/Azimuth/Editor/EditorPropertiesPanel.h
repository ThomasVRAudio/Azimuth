#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Editor/EditorUI.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    class EditorPropertiesPanel
    {

    public:
        static void DrawPanel();

    private:
        static void DrawVec3Box(glm::vec3 &vec3, std::string name, const std::array<std::string, 3> &labels, float speed = 0.01f);
        static void AddComponent();
    };
}

#endif