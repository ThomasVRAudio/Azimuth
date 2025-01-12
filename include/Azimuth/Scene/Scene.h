#pragma once
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/Scripts/ScriptLayer.h>

namespace Azimuth
{
    class Application;
    class MonoScript;
    class Scene
    {

    public:
        Scene(Application &application);
        ~Scene();
        void Init();

        template <typename T>
        T &GetComponent(Entity entity)
        {
            return ECSManager::GetInstance().GetComponent<T>(entity);
        }

        template <typename T>
        void AddComponent(Entity entity, T component)
        {
            ECSManager::GetInstance().AddComponent<T>(entity, component);
        }

        template <typename T>
        void RemoveComponent(Entity entity)
        {
            ECSManager::GetInstance().RemoveComponent<T>(entity);
        }

        Entity CreateEntity()
        {
            Entity entity = ECSManager::GetInstance().CreateEntity();

            return entity;
        }

        void InitScriptsIfNotExist(Entity entity);

        void AddScript(Entity entity, std::shared_ptr<MonoScript> script);

    private:
        static ECSManager &ECS;
        Application &m_Application;
        EditorLayer *m_EditorLayer;
        ScriptLayer *m_ScriptLayer;
    };
}