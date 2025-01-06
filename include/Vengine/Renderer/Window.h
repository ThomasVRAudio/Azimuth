#include <dependencies/glad/glad.h>
#include <dependencies/GLFW/glfw3.h>
#include <Vengine/Common.h>

namespace Vengine
{

    class Window
    {
    public:
        Window(unsigned int width = 800, unsigned int height = 600, const char *name = "Vengine");
        ~Window();
        void OnUpdate();

    private:
        GLFWwindow *m_Window;
        const char *m_Name;
        unsigned int m_Width;
        unsigned int m_Height;
    };

}