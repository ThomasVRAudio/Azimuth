#include <Azimuth/ECS/Components/ScriptContainerComponent.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Scripts/MonoScript.h>

namespace Azimuth
{

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
        {

            print(script->GetParent());
            script->OnStart();
        }
    }

    void ScriptContainerComponent::OnUpdate()
    {
        for (auto &script : m_Scripts)
            script->OnUpdate();
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
        std::filesystem::path relative = std::filesystem::relative(path, Application::projectSettings->ProjectFolder);
        m_Paths.emplace_back(relative);
    }

}