#ifdef AZIMUTH_EDITOR
#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Editor/EditorManager.h>

namespace Azimuth
{
    class EditorFileTrayPanel
    {
    public:
        static void Init();
        static void DrawPanel();

    private:
        inline static std::filesystem::path m_AssetPath = "assets/Assets",
                                            m_CurrentPath = "assets/Assets";
    };
}

#endif