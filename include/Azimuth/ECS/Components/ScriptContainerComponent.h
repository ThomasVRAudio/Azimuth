#pragma once
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/ECS/Components/IComponent.h>

namespace Azimuth
{
    class MonoScript;

    class ScriptContainerComponent : public IComponent
    {
    public:
        ScriptContainerComponent() = default;

        void AddScriptComponent(std::shared_ptr<MonoScript> script);
        void Setup(Scene *ECS, Entity entity);
        void OnStart();
        void OnUpdate();

        template <typename T>
        typename std::enable_if<std::is_base_of_v<IComponent, T>, T &>::type
        GetComponent()
        {
            return m_Scene->GetComponent<T>(m_Entity);
        }

        template <typename T>
        typename std::enable_if<std::is_base_of_v<IComponent, T>, void>::type
        AddComponent()
        {
            T component;
            return m_Scene->AddComponent<T>(m_Entity, component);
        }

        template <typename T>
        typename std::enable_if<std::is_base_of_v<IComponent, T>, void>::type
        RemoveComponent()
        {
            return m_Scene->RemoveComponent<T>(m_Entity);
        }

        std::vector<std::filesystem::path> GetScriptPaths()
        {
            return m_Paths;
        }

        void AddScriptPath(const std::filesystem::path &path)
        {
            std::filesystem::path relative = std::filesystem::relative(path, Application::projectSettings->ProjectFolder);
            m_Paths.emplace_back(relative);
        }

        virtual void GetDebugSettings();

    private:
        std::vector<std::shared_ptr<MonoScript>> m_Scripts;
        std::vector<std::filesystem::path> m_Paths;
        Entity m_Entity;
        Scene *m_Scene;
    };

}