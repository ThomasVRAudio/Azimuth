#pragma once
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Scene/SceneSettings.h>

namespace Azimuth
{
    class MonoScript;

#ifdef AZIMUTH_EDITOR
    class EditorManager;
#endif

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

        template <typename T>
        bool HasComponent(Entity entity)
        {
            return ECS->HasComponent<T>(entity);
        }

        Entity CreateEntity(std::string name);
        void DestroyEntity(Entity entity);

        inline ECSManager *GetECSManager() { return ECS; }
        std::shared_ptr<SceneSettings> Settings = std::make_shared<SceneSettings>();

    private:
        ECSManager *ECS = new ECSManager();
        Application &m_Application;
        std::vector<Entity> m_Entities;
#ifdef AZIMUTH_EDITOR
        friend EditorManager;
        friend class Serializer;
        friend class EditorHierarchyPanel;
#endif
    };
}