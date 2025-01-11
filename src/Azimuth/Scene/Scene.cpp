#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Core/Application.h>

namespace Azimuth
{
    ECSManager &Scene::ECS = ECSManager::GetInstance();

    Scene::Scene(Application &application)
        : m_Application(application) {
          };

    void Scene::Init()
    {
        ECS.Init();
        ECS.RegisterComponent<TransformComponent>();
        ECS.RegisterComponent<MeshComponent>();
        ECS.RegisterComponent<AudioComponent>();
        ECS.RegisterComponent<ScriptsComponent>();

        m_EditorLayer = new EditorLayer();
        m_ScriptLayer = new ScriptLayer();
        m_Application.AddLayer(m_EditorLayer);
        m_Application.AddLayer(m_ScriptLayer);
    }

    Scene::~Scene()
    {
        delete m_EditorLayer;
        delete m_ScriptLayer;
    }
}