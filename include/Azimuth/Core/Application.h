#pragma once

#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Common.h>
#include <Azimuth/Scene/Scene.h>

namespace Azimuth
{
    class Application
    {
    public:
        Application();
        ~Application();
        void Start();
        void Run();
        void AddLayer(Layer *layer);

    private:
        bool m_isRunning = false;
        std::unique_ptr<Window> m_Window;
        std::vector<Layer *> m_Layers;
        Scene scene;
    };
}