#include <Azimuth/Scripts/ScriptSystem.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    void ScriptSystem::Init()
    {
    }

    void ScriptSystem::OnStart()
    {
        ECSManager &ECS = ECSManager::GetInstance();

        for (auto &entity : m_Entities)
        {
            ECS.GetComponent<ScriptsComponent>(entity).OnStart();
        }
    }

    void ScriptSystem::OnUpdate()
    {
        ECSManager &ECS = ECSManager::GetInstance();

        for (auto &entity : m_Entities)
        {
            ECS.GetComponent<ScriptsComponent>(entity).OnUpdate();
        };
    }
}