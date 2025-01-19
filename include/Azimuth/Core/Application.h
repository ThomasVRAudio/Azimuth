#pragma once

#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Game/GameModeLayer.h>
#include <Azimuth/Scene/Scene.h>

#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorLayer.h>
#endif

namespace Azimuth
{
    class Scene;

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
#ifdef AZIMUTH_EDITOR
        static bool s_PlayingEditorScene;
        static bool s_IsFirstPlayFrame;
#endif

    private:
        bool m_isRunning = false;
        Layer *m_GameLayer;
        std::vector<Layer *> m_Layers;
    };

#ifdef AZIMUTH_EDITOR
    inline bool Application::s_PlayingEditorScene = false;
    inline bool Application::s_IsFirstPlayFrame = true;
#endif
}