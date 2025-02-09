#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/System/Files.h>
#include <Azimuth/Editor/EditorUI.h>
#include <Azimuth/Renderer/Model.h>

namespace Azimuth
{
    class EditorFilepicker
    {
    public:
        EditorFilepicker() = default;
        void DrawPanel(const char *path, std::string panelName, bool dockable);
        inline void SetOpenWindow(bool show) { m_IsOpen = show; };
        inline bool IsOpen() { return m_IsOpen; };

    private:
        bool m_IsOpen = false;
    };
}

#endif