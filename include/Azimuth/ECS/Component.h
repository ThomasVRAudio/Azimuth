#pragma once

#include <dependencies/glm/glm.hpp>
#include <Azimuth/Common.h>
#include <Azimuth/Scene/Scene.h>

namespace Azimuth
{

    struct IComponent
    {
    };

    struct TransformComponent : public IComponent
    {
        glm::vec3 Position;
    };

    struct MeshComponent : public IComponent
    {
    };

    struct AudioComponent : public IComponent
    {
    };

    class MonoScript
    {
    public:
        virtual void OnStart() = 0;
        virtual void OnUpdate() = 0;

        template <typename T>
        typename std::enable_if<std::is_base_of<IComponent, T>::value, T &>::type
        GetComponent()
        {
            return m_Scene->GetComponent<T>(m_Entity);
        }

        template <typename T>
        typename std::enable_if<std::is_base_of<IComponent, T>::value, void>::type
        AddComponent()
        {
            T component;
            return m_Scene->AddComponent<T>(m_Entity, component);
        }

        template <typename T>
        typename std::enable_if<std::is_base_of<IComponent, T>::value, void>::type
        RemoveComponent()
        {
            return m_Scene->RemoveComponent<T>(m_Entity);
        }

        void SetScene(Scene *scene) { m_Scene = scene; }
        void SetEntity(Entity entity) { m_Entity = entity; }

    private:
        Scene *m_Scene;
        Entity m_Entity = 0;
    };

    class ScriptsComponent : public IComponent
    {
    public:
        ScriptsComponent()
        {
        }

        void AddScript(std::shared_ptr<MonoScript> script)
        {
            m_Scripts.emplace_back(std::move(script));
        }

        void OnStart()
        {
            for (auto &script : m_Scripts)
                script->OnStart();
        }

        void OnUpdate()
        {
            for (auto &script : m_Scripts)
                script->OnUpdate();
        }

    private:
        std::vector<std::shared_ptr<MonoScript>> m_Scripts;
    };

}