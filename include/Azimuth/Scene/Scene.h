#pragma once
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/Scripts/ScriptLayer.h>

namespace Azimuth
{
    class Application;
    class MonoScript;
    class EditorLayer;

    class Scene
    {

    public:
        Scene(Application &application);
        ~Scene();
        void Init();

        template <typename T>
        T &GetComponent(Entity entity)
        {
            return ECS->GetComponent<T>(entity);
        }

        template <typename T>
        void AddComponent(Entity entity, T component)
        {
            ECS->AddComponent<T>(entity, component);
        }

        template <typename T>
        void RemoveComponent(Entity entity)
        {
            ECS->RemoveComponent<T>(entity);
        }

        Entity CreateEntity()
        {
            Entity entity = ECS->CreateEntity();
            return entity;
        }

        void InitScriptsIfNotExist(Entity entity);

        void AddScript(Entity entity, std::shared_ptr<MonoScript> script);
        inline ECSManager *GetECSManager() { return ECS; }

    private:
        ECSManager *ECS = new ECSManager();
        Application &m_Application;

        ScriptLayer *m_ScriptLayer;
    };
}