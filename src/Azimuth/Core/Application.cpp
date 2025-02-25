#include <Azimuth/Core/Application.h>
#include <Azimuth/Core/Time.h>
#include <Azimuth/Core/Input.h>
#include <Azimuth/Game/GameModeLayer.h>
#include <Azimuth/Scripts/ScriptLayer.h>

namespace Azimuth
{

    Application::Application()
        : m_isRunning(true)
    {
        m_ActiveScene = new Scene(*this);
        Init();
    }

    Application::~Application()
    {
        for (auto const layer : m_Layers)
            delete layer;

        if (m_ActiveScene)
            delete m_ActiveScene;

        if (m_GameLayer)
            delete m_GameLayer;

        if (m_ScriptLayer)
            delete m_ScriptLayer;
    }

    void Application::Init()
    {
        Time::StartGlobalTime();
        Time::AddTimeStep();
        Window::Create();

        g_ScrollEvent.InitializeCallbacks();
        g_CursorEvent.InitializeCallbacks();

        m_ActiveScene->Init();

#ifdef AZIMUTH_EDITOR
        m_GameLayer = new EditorLayer();
#else
        m_GameLayer = new GameModeLayer();
#endif
        AddLayer(m_GameLayer);

        m_ScriptLayer = new ScriptLayer();
        AddLayer(m_ScriptLayer);

        for (auto const layer : m_Layers)
        {
            layer->Init(m_ActiveScene);
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