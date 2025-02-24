#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorManager.h>

namespace Azimuth
{
    class EditorSettingsPanel
    {
    public:
        static void DrawPanel();

    private:
        inline static bool m_VSyncLastCheckboxState = false;
        inline static bool m_PreviousPlayingState = false;
    };

}

#endif