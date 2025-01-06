#pragma once

#include <iostream>

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
    };
}