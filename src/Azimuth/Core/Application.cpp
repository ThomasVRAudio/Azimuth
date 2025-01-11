#include "Azimuth/Core/Application.h"

namespace Azimuth
{

    Application::Application()
        : m_isRunning(true)
    {
        MainScene = new Scene(*this);
        m_Window = std::make_unique<Window>();
        Init();
    }

    Application::~Application()
    {
        for (auto const layer : m_Layers)
        {
            delete layer;
        }

        delete MainScene;
    }

    void Application::Init()
    {
        MainScene->Init();
        for (auto const layer : m_Layers)
        {
            layer->Init();
        }
    }

    void Application::Start()
    {
        for (auto const layer : m_Layers)
        {
            layer->OnStart();
        }
    }

    void Application::Run()
    {
        Start();
        while (m_isRunning)
        {
            for (auto const layer : m_Layers)
            {
                layer->OnUpdate();
            }

            m_Window->OnUpdate();
        }
    }

    void Application::AddLayer(Layer *layer)
    {
        m_Layers.emplace_back(layer);
    }
}