#pragma once
#include <Azimuth/ECS/Components/IComponent.h>
#include <Azimuth/ECS/Components/ScriptContainerComponent.h>
#include <Azimuth/Common.h>

namespace Azimuth
{
    class MonoScript
    {
    public:
        MonoScript(const std::filesystem::path &path)
            : m_Path(path)
        {
        }

        virtual void OnStart() {};
        virtual void OnUpdate() {};
        virtual std::shared_ptr<MonoScript> Clone() const = 0;

        template <typename T>
        typename std::enable_if<std::is_base_of_v<IComponent, T>, T &>::type
        GetComponent()
        {
            return m_ScriptContainer->GetComponent<T>();
        }

        template <typename T>
        typename std::enable_if<std::is_base_of_v<IComponent, T>, void>::type
        AddComponent()
        {
            T component;
            return m_ScriptContainer->AddComponent<T>();
        }

        template <typename T>
        typename std::enable_if<std::is_base_of_v<IComponent, T>, void>::type
        RemoveComponent()
        {
            return m_ScriptContainer->RemoveComponent<T>();
        }

        void SetParent(ScriptContainerComponent *parent)
        {
            m_ScriptContainer = parent;
        }

        ScriptContainerComponent *GetParent() { return m_ScriptContainer; }

        const std::filesystem::path &GetPath() const { return m_Path; }
        void SetPath(const std::filesystem::path &path)
        {
            m_Path = path;
        }

    private:
        std::filesystem::path m_Path;
        ScriptContainerComponent *m_ScriptContainer;
    };
}