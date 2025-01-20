#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Core/Application.h>

namespace Azimuth
{
    Scene::Scene(Application &application)
        : m_Application(application) {
          };

    void Scene::Init()
    {
        ECS->Init();
        ECS->RegisterComponent<TransformComponent>();
        ECS->RegisterComponent<MeshComponent>();
        ECS->RegisterComponent<AudioComponent>();
        ECS->RegisterComponent<ScriptsComponent>();
        ECS->RegisterComponent<TagComponent>();

        m_ScriptLayer = new ScriptLayer();
        m_Application.AddLayer(m_ScriptLayer);
    }

    Scene::~Scene()
    {
        if (m_ScriptLayer)
            delete m_ScriptLayer;

        if (ECS)
            delete ECS;
    }

    void Scene::InitScriptsIfNotExist(Entity entity)
    {
        bool hasScriptBase = ECS->HasComponent<ScriptsComponent>(entity);

        if (!hasScriptBase)
        {
            ScriptsComponent component;
            ECS->AddComponent<ScriptsComponent>(entity, std::move(component));
        }
    }

    void Scene::AddScript(Entity entity, std::shared_ptr<MonoScript> script)
    {
        InitScriptsIfNotExist(entity);
        ScriptsComponent &baseComponent = ECS->GetComponent<ScriptsComponent>(entity);

        if (!script->HasParent())
        {
            std::shared_ptr sharedBase = std::make_shared<ScriptsComponent>(baseComponent);
            sharedBase->m_Scene = this;
            sharedBase->m_Entity = entity;
            script->SetParent(sharedBase);
        }

        baseComponent.AddScript(script);
    }

    Entity Scene::CreateEntity(std::string name)
    {
        Entity entity = ECS->CreateEntity();
        TagComponent tagComponent;

        ECS->AddComponent<TagComponent>(entity, tagComponent);
        ECS->GetComponent<TagComponent>(entity).name = name;

        m_Entities.emplace_back(entity);
        return entity;
    }

}