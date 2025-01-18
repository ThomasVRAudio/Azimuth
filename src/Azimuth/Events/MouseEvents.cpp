#include <Azimuth/Events/MouseEvents.h>

namespace Azimuth
{
    void MouseScrollEvent::InitializeCallbacks()
    {
        glfwSetScrollCallback(Window::GetMainWindow(), [](GLFWwindow *window, double xOffset, double yOffset)
                              { g_ScrollEvent.Dispatch(xOffset, yOffset); });
    }

    void MouseCursorEvent::InitializeCallbacks()
    {
        glfwSetCursorPosCallback(Window::GetMainWindow(), [](GLFWwindow *window, double xOffset, double yOffset)
                                 { g_CursorEvent.Dispatch(xOffset, yOffset); });
    }
}