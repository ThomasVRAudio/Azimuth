#pragma once

#include <Azimuth/Common.h>

namespace Azimuth
{

    class Window
    {
    public:
        Window(unsigned int width = 800, unsigned int height = 600, const char *name = "Azimuth");
        ~Window();
        void OnUpdate();

    private:
        GLFWwindow *m_Window;
        const char *m_Name;
        unsigned int m_Width;
        unsigned int m_Height;
    };

}