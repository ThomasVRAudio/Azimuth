#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorUI.h>

namespace Azimuth
{
    class EditorSettingsPanel
    {
    public:
        static void DrawPanel();
        inline static float Exposure = 1.0f;
    };

}

#endif