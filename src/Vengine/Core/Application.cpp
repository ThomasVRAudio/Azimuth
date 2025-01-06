#include "Vengine/Core/Application.h"

namespace Vengine
{

    Application::Application()
        : m_isRunning(true)
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_isRunning)
        {
        }
    }

}