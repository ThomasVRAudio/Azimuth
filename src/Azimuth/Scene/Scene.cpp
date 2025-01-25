#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/ECS/Components/ScriptComponent.h>

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
        ECS->RegisterComponent<ScriptComponent>();
        ECS->RegisterComponent<TagComponent>();
        ECS->RegisterComponent<MaterialComponent>();
        ECS->RegisterComponent<LightComponent>();

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
        bool hasScriptBase = ECS->HasComponent<ScriptComponent>(entity);

        if (!hasScriptBase)
        {
            ScriptComponent component;
            ECS->AddComponent<ScriptComponent>(entity, std::move(component));
        }
    }

    void Scene::AddScript(Entity entity, std::shared_ptr<MonoScript> script)
    {
        InitScriptsIfNotExist(entity);
        ScriptComponent &baseComponent = ECS->GetComponent<ScriptComponent>(entity);

        if (!script->HasParent())
        {
            std::shared_ptr sharedBase = std::make_shared<ScriptComponent>(baseComponent);
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
        TransformComponent transformComponent;

        ECS->AddComponent<TagComponent>(entity, std::move(tagComponent));
        ECS->AddComponent<TransformComponent>(entity, std::move(transformComponent));
        ECS->GetComponent<TagComponent>(entity).name = name;

        m_Entities.emplace_back(entity);
        return entity;
    }

}