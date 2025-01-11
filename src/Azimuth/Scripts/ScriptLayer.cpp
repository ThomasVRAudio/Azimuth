#include <Azimuth/Scripts/ScriptLayer.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{

    void ScriptLayer::Init()
    {
        ECSManager &ECS = ECSManager::GetInstance();

        ComponentMask mask;

        mask.set(ECS.GetComponentBitType<ScriptsComponent>(), true);
        m_ScriptSystem = ECS.RegisterSystem<ScriptSystem>();
        ECS.SetSystemComponentMask<ScriptSystem>(mask);
        m_ScriptSystem->Init();
    }

    void ScriptLayer::OnStart()
    {
        m_ScriptSystem->OnStart();
    }

    void ScriptLayer::OnUpdate()
    {
        m_ScriptSystem->OnUpdate();
    }

}