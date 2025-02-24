#include <Azimuth/Scripts/ScriptSystem.h>
#include <Azimuth/ECS/Components/ScriptContainerComponent.h>

namespace Azimuth
{
    void ScriptSystem::Init(ECSManager *ECS)
    {
        this->ECS = ECS;
    }

    void ScriptSystem::OnStart()
    {
        for (auto &entity : m_Entities)
        {
            auto &container = ECS->GetComponent<ScriptContainerComponent>(entity);
            container.OnStart();
        }
    }

    void ScriptSystem::OnUpdate()
    {
        for (auto &entity : m_Entities)
        {
            ECS->GetComponent<ScriptContainerComponent>(entity).OnUpdate();
        };
    }
}