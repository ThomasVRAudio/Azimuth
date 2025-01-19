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
            delete layer;

        if (ActiveScene)
            delete ActiveScene;

        if (m_GameLayer)
            delete m_GameLayer;
    }

    void Application::Init()
    {
        Time::StartGlobalTime();
        Time::AddTimeStep();
        Window::Create();

        g_ScrollEvent.InitializeCallbacks();
        g_CursorEvent.InitializeCallbacks();

        ActiveScene->Init();

#ifdef AZIMUTH_EDITOR
        m_GameLayer = new EditorLayer();
#else
        m_GameLayer = new GameModeLayer();
#endif
        AddLayer(m_GameLayer);

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