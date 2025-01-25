#include <Azimuth/Scripts/ScriptSystem.h>
#include <Azimuth/ECS/Components/ScriptComponent.h>

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
            ECS->GetComponent<ScriptComponent>(entity).OnStart();
        }
    }

    void ScriptSystem::OnUpdate()
    {
        for (auto &entity : m_Entities)
        {
            ECS->GetComponent<ScriptComponent>(entity).OnUpdate();
        };
    }
}