#pragma once
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

    class MonoScript;

    class ScriptsComponent : public IComponent
    {
    public:
        ScriptsComponent() = default;

        void AddScript(std::shared_ptr<MonoScript> script)
        {
            m_Scripts.emplace_back(std::move(script));
        }

        void OnStart();
        void OnUpdate();

    private:
        std::vector<std::shared_ptr<MonoScript>> m_Scripts;
        Scene *m_Scene;
        Entity m_Entity = 0;
        friend MonoScript;
    };

    class MonoScript : public IComponent
    {
    public:
        virtual void OnStart() = 0;
        virtual void OnUpdate() = 0;

        template <typename T>
        typename std::enable_if<std::is_base_of<IComponent, T>::value, T &>::type
        GetComponent()
        {
            return m_ScriptParent->m_Scene->GetComponent<T>(m_ScriptParent->m_Entity);
        }

        template <typename T>
        typename std::enable_if<std::is_base_of<IComponent, T>::value, void>::type
        AddComponent()
        {
            T component;
            return m_ScriptParent->m_Scene->AddComponent<T>(m_ScriptParent->m_Entity, component);
        }

        template <typename T>
        typename std::enable_if<std::is_base_of<IComponent, T>::value, void>::type
        RemoveComponent()
        {
            return m_ScriptParent->m_Scene->RemoveComponent<T>(m_ScriptParent->m_Entity);
        }

        void SetParent(std::shared_ptr<ScriptsComponent> parent)
        {
            m_ScriptParent = parent;
        }

        bool HasParent()
        {
            return m_ScriptParent != nullptr;
        }

    private:
        std::shared_ptr<ScriptsComponent> m_ScriptParent;
    };

}