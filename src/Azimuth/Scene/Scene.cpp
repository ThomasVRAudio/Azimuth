#include <Azimuth/Scene/Scene.h>
#include <Azimuth/ECS/Components/ScriptContainerComponent.h>

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
        ECS->RegisterComponent<ScriptContainerComponent>();
        ECS->RegisterComponent<TagComponent>();
        ECS->RegisterComponent<MaterialComponent>();
        ECS->RegisterComponent<LightComponent>();
    }

    Entity Scene::CreateEntity(std::string name)
    {
        Entity entity = ECS->CreateEntity();
        TagComponent tagComponent;
        TransformComponent transformComponent;
        ScriptContainerComponent scriptContainerComponent;
        scriptContainerComponent.Setup(this, entity);

        ECS->AddComponent<TagComponent>(entity, std::move(tagComponent));
        ECS->AddComponent<TransformComponent>(entity, std::move(transformComponent));
        ECS->GetComponent<TagComponent>(entity).name = name;
        ECS->AddComponent<ScriptContainerComponent>(entity, std::move(scriptContainerComponent));

        m_Entities.emplace_back(entity);
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        ECS->DestroyEntity(entity);
    }

    void Scene::ClearScene()
    {
        for (auto &entity : m_Entities)
            DestroyEntity(entity);

        m_Entities.clear();
    }

}