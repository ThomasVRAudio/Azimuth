#include "Vengine/Core/Application.h"

namespace Vengine
{

    Application::Application()
        : m_isRunning(true)
    {
        m_Window = std::make_unique<Window>();
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_isRunning)
        {
            m_Window->OnUpdate();
        }
    }

}