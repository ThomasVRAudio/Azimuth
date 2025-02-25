#pragma once
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Project/ProjectSettings.h>
#include <Azimuth/Common.h>

#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorLayer.h>
#endif

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
        inline static Scene *GetActiveScene() { return m_ActiveScene; }
        inline static std::shared_ptr<Azimuth::ProjectSettings> projectSettings =
            std::make_shared<Azimuth::ProjectSettings>(std::filesystem::path(R"(D:\Users\Thomas\Documents\Dev\Azimuth_Engine\ProjectAssets)"));

#ifdef AZIMUTH_EDITOR
        inline static bool s_PlayingEditorScene = false;
        inline static bool s_IsFirstPlayFrame = true;
#endif

    private:
        inline static Scene *m_ActiveScene;
        bool m_isRunning = false;
        Layer *m_GameLayer, *m_ScriptLayer;
        std::vector<Layer *> m_Layers;
    };
}