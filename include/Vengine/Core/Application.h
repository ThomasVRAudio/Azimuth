#pragma once
#include <Vengine/Renderer/Window.h>
#include <iostream>
#include <memory>

namespace Vengine
{
    class Application
    {
    public:
        Application();
        ~Application();
        bool IsRunning() { return m_isRunning; };
        void Run();

    private:
        bool m_isRunning = false;
        std::unique_ptr<Window> m_Window;
    };
}