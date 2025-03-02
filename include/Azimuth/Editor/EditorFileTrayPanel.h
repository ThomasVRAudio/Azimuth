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
        inline static std::filesystem::path m_AssetPath, m_CurrentPath, m_PreviousPath;
        inline static std::function<void()> m_OnTextSubmitFunc = nullptr;
        inline static char m_InputText[256] = "";
        inline static std::string m_PopupTitle;
        static void ClearPopup();
        inline static std::vector<std::filesystem::directory_entry> m_Files;
        inline static std::vector<std::filesystem::directory_entry> m_Folders;
        inline static float m_Time;
    };
}

#endif