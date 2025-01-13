#pragma once
#include <Azimuth/Core/KeyCodes.h>
#include <Azimuth/Core/MouseButtonCodes.h>
#include <Azimuth/Renderer/Window.h>

namespace Azimuth
{
    class Application;
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key)
        {
            auto keyState = glfwGetKey(Window::m_Window, static_cast<int>(key));
            return keyState == GLFW_PRESS;
        }

        static bool IsMouseButtonPressed(MouseButtonCode mouseButton)
        {
            auto mouseState = glfwGetMouseButton(Window::m_Window, static_cast<int>(mouseButton));
            return mouseState == GLFW_PRESS;
        }

        static glm::vec2 GetMouseXY()
        {
            glfwGetCursorPos(Window::m_Window, &mousePosX, &mousePosY);
            return {mousePosX, mousePosY};
        }

        static double GetMouseX()
        {
            glfwGetCursorPos(Window::m_Window, &mousePosX, &mousePosY);
            return mousePosX;
        }

        static double GetMouseY()
        {
            glfwGetCursorPos(Window::m_Window, &mousePosX, &mousePosY);
            return mousePosY;
        }

        static double GetMouseXScreen()
        {
            return GetMouseX() / Window::GetWidth();
        }

        static double GetMouseYScreen()
        {
            return -(GetMouseY() / Window::GetHeight());
        }

        static glm::vec2 GetMouseXYScreen()
        {
            auto mousePos = GetMouseXY();
            return {mousePos.x / Window::GetWidth(), 1.0f - (mousePos.y / Window::GetHeight())};
        }

    private:
        static double mousePosX, mousePosY;
    };
}