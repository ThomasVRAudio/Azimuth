#pragma once
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/Scripts/ScriptLayer.h>

namespace Azimuth
{
    class MonoScript;
    class Application;
    class EditorLayer;

#ifdef AZIMUTH_EDITOR
    class EditorUI;
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

        void InitScriptsIfNotExist(Entity entity);

        void AddScript(Entity entity, std::shared_ptr<MonoScript> script);
        inline ECSManager *GetECSManager() { return ECS; }

    private:
        ECSManager *ECS = new ECSManager();
        Application &m_Application;
        ScriptLayer *m_ScriptLayer;
        std::vector<Entity> m_Entities;
#ifdef AZIMUTH_EDITOR
        friend EditorUI;
        friend class EditorHierarchyPanel;
#endif
    };
}