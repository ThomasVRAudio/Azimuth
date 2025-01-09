#pragma once

#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Common.h>
#include <Azimuth/ECS/ECSManager.h>

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
        ECSManager &ECS = ECSManager::getInstance();

    private:
        bool m_isRunning = false;
        std::unique_ptr<Window> m_Window;
        std::vector<Layer *> m_Layers;
    };
}