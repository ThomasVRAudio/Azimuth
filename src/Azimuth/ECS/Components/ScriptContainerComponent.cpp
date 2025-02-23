#include <Azimuth/ECS/Components/ScriptContainerComponent.h>
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
        m_IDs.emplace_back(script->GetID());
        m_Scripts.emplace_back(script);
    }

    void ScriptContainerComponent::OnStart()
    {
        if (m_Scene == nullptr)
        {
            print("Scene not initialized");
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
}