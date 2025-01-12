#include "Azimuth/Renderer/Window.h"

namespace Azimuth
{
    Window::Window(unsigned int width, unsigned int height, const char *name)
        : m_Width(width), m_Height(height), m_Name(name)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_SAMPLES, 4);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Name, NULL, NULL);

        if (m_Window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        glfwMakeContextCurrent(m_Window);

        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *m_Window, int width, int height)
                                       { glViewport(0, 0, width, height); });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
    }

    Window::~Window()
    {
        if (m_Window)
            glfwDestroyWindow(m_Window);
    }

    void Window::OnUpdate()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}
