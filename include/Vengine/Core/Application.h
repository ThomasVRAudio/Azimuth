#pragma once

#include <Vengine/Renderer/Window.h>
#include <Vengine/Core/Layer.h>
#include <Vengine/Common.h>

namespace Vengine
{
    class Application
    {
    public:
        Application();
        ~Application();
        bool IsRunning() { return m_isRunning; };
        void Start();
        void Run();
        void AddLayer(Layer *layer);

    private:
        bool m_isRunning = false;
        std::unique_ptr<Window> m_Window;
        std::vector<Layer *> m_Layers;
    };
}