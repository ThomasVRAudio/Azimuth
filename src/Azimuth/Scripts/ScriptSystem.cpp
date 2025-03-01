#include <Azimuth/Scripts/ScriptSystem.h>
#include <Azimuth/ECS/Components/ScriptContainerComponent.h>

namespace Azimuth
{
    void ScriptSystem::Init(Scene *scene)
    {
        m_Scene = scene;
    }

    void ScriptSystem::OnStart()
    {
        ECSManager *ECS = m_Scene->GetECSManager();
        for (auto &entity : m_Entities)
        {
            auto &container = ECS->GetComponent<ScriptContainerComponent>(entity);
            container.OnStart();
        }
    }

    void ScriptSystem::OnUpdate()
    {
        ECSManager *ECS = m_Scene->GetECSManager();

        for (auto &entity : m_Entities)
            ECS->GetComponent<ScriptContainerComponent>(entity).OnUpdate();
    }
}