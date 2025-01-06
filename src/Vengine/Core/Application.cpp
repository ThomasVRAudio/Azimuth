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
        for (auto const layer : m_Layers)
        {
            delete layer;
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