#pragma once
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/Scripts/ScriptLayer.h>

namespace Azimuth
{
    class Application;
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

        static ECSManager &ECS;

    private:
        Application &m_Application;
        EditorLayer *m_EditorLayer;
        ScriptLayer *m_ScriptLayer;
    };
}