#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Editor/EditorManager.h>

namespace Azimuth
{
    class EditorHierarchyPanel
    {
    public:
        static void DrawPanel();
    };
}
#endif