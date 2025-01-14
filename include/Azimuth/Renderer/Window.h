#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{

    class Input;
    class EditorLayer;
    class Window
    {
    public:
        static void Create(unsigned int width = 800, unsigned int height = 600, const char *name = "Azimuth");
        static void OnUpdate();
        static float GetWidth() { return m_Width; }
        static float GetHeight() { return m_Height; }

    private:
        static GLFWwindow *m_Window;
        static const char *m_Name;
        static unsigned int m_Width;
        static unsigned int m_Height;
        friend Input;
        friend EditorLayer;
    };

}