#include "Azimuth/Renderer/Window.h"

namespace Azimuth
{
    ImVec4 Window::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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

        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *m_Window, int m_Width, int m_Height)
                                       { glViewport(0, 0, m_Width, m_Height); });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void Window::OnUpdate()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::ColorEdit3("clear color", (float *)&clear_color);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}
