#ifdef AZIMUTH_EDITOR
#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    class EditorFileTrayPanel
    {
    public:
        static void Init();
        static void DrawPanel();

    private:
        inline static std::filesystem::path m_AssetPath, m_CurrentPath;
    };
}

#endif