#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorUI.h>

namespace Azimuth
{
    class EditorSettingsPanel
    {
    public:
        static void DrawPanel();

    private:
        inline static bool m_VSyncCheckboxState = false;
        inline static bool m_VSyncLastCheckboxState = false;
    };

}

#endif