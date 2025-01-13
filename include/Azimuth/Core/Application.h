#pragma once

#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Scene/Scene.h>

namespace Azimuth
{
    class Application
    {
    public:
        Application();
        ~Application();
        void Init();
        void Start();
        void Run();
        void AddLayer(Layer *layer);
        Scene *ActiveScene;

    private:
        bool m_isRunning = false;
        std::vector<Layer *> m_Layers;
    };
}