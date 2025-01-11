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

    void Scene::InitScriptsIfNotExist(Entity entity)
    {
        ECSManager &ECS = ECSManager::GetInstance();
        bool hasScriptBase = ECS.HasComponent<ScriptsComponent>(entity);

        if (!hasScriptBase)
        {
            ScriptsComponent component;
            ECS.AddComponent<ScriptsComponent>(entity, std::move(component));
        }
    }

    void Scene::AddScript(Entity entity, std::shared_ptr<MonoScript> script)
    {
        InitScriptsIfNotExist(entity);
        ECSManager &ECS = ECSManager::GetInstance();
        ScriptsComponent &baseComponent = ECS.GetComponent<ScriptsComponent>(entity);

        if (!script->HasParent())
        {
            std::shared_ptr sharedBase = std::make_shared<ScriptsComponent>(baseComponent);
            script->SetParent(sharedBase);
        }

        baseComponent.AddScript(script);
    }

}