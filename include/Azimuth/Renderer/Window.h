#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    class Window
    {
    public:
        static void Create(unsigned int width = 1600, unsigned int height = 900, const char *name = "Azimuth");
        static void OnUpdate();
        static float GetWidth() { return m_Width; }
        static float GetHeight() { return m_Height; }
        static void SetVSync(boolean on);

        inline static GLFWwindow *GetMainWindow() { return m_Window; }

    private:
        static GLFWwindow *m_Window;
        static const char *m_Name;
        static unsigned int m_Width;
        static unsigned int m_Height;
    };
}