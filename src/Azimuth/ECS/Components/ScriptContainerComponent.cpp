#include <Azimuth/ECS/Components/ScriptContainerComponent.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Scripts/MonoScript.h>

namespace Azimuth
{

    ScriptContainerComponent::ScriptContainerComponent(const ScriptContainerComponent &other)
        : m_Paths(other.m_Paths), m_Scene(other.m_Scene), m_Entity(other.m_Entity)
    {
        for (const auto &ptr : other.m_Scripts)
            m_Scripts.push_back(ptr->Clone());
    }

    void ScriptContainerComponent::Setup(Scene *scene, Entity entity)
    {
        m_Entity = entity;
        m_Scene = scene;
    }

    void ScriptContainerComponent::AddScriptComponent(std::shared_ptr<MonoScript> script)
    {
        script->SetParent(this);
        m_Scripts.emplace_back(script);
    }

    void ScriptContainerComponent::OnStart()
    {
        if (m_Scene == nullptr)
        {
            print("Scripts::Error Scene not initialized");
            return;
        }

        for (auto &script : m_Scripts)
            script->OnStart();
    }

    void ScriptContainerComponent::OnUpdate()
    {
        for (auto &script : m_Scripts)
            script->OnUpdate();
    }

    void ScriptContainerComponent::ClearScripts()
    {
        for (auto &ptr : m_Scripts)
            ptr.reset();

        m_Scripts.clear();
    }

    void ScriptContainerComponent::GetDebugSettings()
    {
        for (const auto &script : m_Scripts)
            print("path of script: " << script->GetPath());

        print("Entity: " << m_Entity);
        print("Scene has transform: " << m_Scene->HasComponent<TransformComponent>(m_Entity));
    }

    void ScriptContainerComponent::AddScriptPath(const std::filesystem::path &path)
    {
        m_Paths.emplace_back(path);
    }

    void ScriptContainerComponent::ReplaceScriptPathRelative(const std::filesystem::path &currentPath, const std::filesystem::path &newPath)
    {
        auto it = std::find(m_Paths.begin(), m_Paths.end(), currentPath);
        if (it != m_Paths.end())
            *it = newPath.filename().stem().string() + ".h";
    }
}