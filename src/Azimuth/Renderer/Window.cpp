#include "Azimuth/Renderer/Window.h"

namespace Azimuth
{
    GLFWwindow *Window::m_Window;
    unsigned int Window::m_Width;
    unsigned int Window::m_Height;
    const char *Window::m_Name;

    void Window::Create(unsigned int width, unsigned int height, const char *name)
    {
        m_Width = width;
        m_Height = height;
        m_Name = name;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_SAMPLES, 4);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Name, NULL, NULL);

        if (m_Window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        glfwMakeContextCurrent(m_Window);

        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *m_Window, int m_Width, int m_Height)
                                       { glViewport(0, 0, m_Width, m_Height); });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
    }

    void Window::OnUpdate()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}
