#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>

namespace Azimuth
{
    class EditorFilepicker
    {
    public:
        EditorFilepicker() = default;
        const std::string SelectFile(const char *path, std::string panelName, const std::vector<std::string> &fileExtensions, bool dockable, std::function<void()> selectionMenuCallback = nullptr);
        inline void SetOpenWindow(bool show) { m_IsOpen = show; };
        inline bool IsOpen() { return m_IsOpen; };

    private:
        bool m_IsOpen = false;
    };
}

#endif