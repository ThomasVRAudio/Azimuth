#pragma once
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Scene/SceneSettings.h>

namespace Azimuth
{
    class MonoScript;
    class Application;

#ifdef AZIMUTH_EDITOR
    class EditorManager;
#endif

    class Scene
    {
    public:
        Scene(Application &application);
        ~Scene() = default;
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
        void ClearScene();

        inline ECSManager *GetECSManager() { return ECS.get(); }
        inline void SetECSManager(std::unique_ptr<ECSManager> ECS) { this->ECS = std::move(ECS); }
        std::shared_ptr<SceneSettings> Settings = std::make_shared<SceneSettings>();
        inline std::vector<Entity> GetSceneEntities() { return m_Entities; }

    private:
        std::unique_ptr<ECSManager> ECS = std::make_unique<ECSManager>();
        Application &m_Application;
        std::vector<Entity> m_Entities;
#ifdef AZIMUTH_EDITOR
        friend EditorManager;
        friend class EditorHierarchyPanel;
#endif
    };
}