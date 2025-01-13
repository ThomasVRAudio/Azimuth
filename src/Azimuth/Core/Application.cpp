#include <Azimuth/Core/Application.h>
#include <Azimuth/Core/Time.h>
#include <Azimuth/Core/Input.h>

namespace Azimuth
{

    Application::Application()
        : m_isRunning(true)
    {
        ActiveScene = new Scene(*this);
        Init();
    }

    Application::~Application()
    {
        for (auto const layer : m_Layers)
        {
            delete layer;
        }

        delete ActiveScene;
    }

    void Application::Init()
    {
        Time::StartGlobalTime();
        Time::AddTimeStep();
        Window::Create();

        ActiveScene->Init();
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

            Window::OnUpdate();
            Time::AddTimeStep();
        }
    }

    void Application::AddLayer(Layer *layer)
    {
        m_Layers.emplace_back(layer);
    }
}