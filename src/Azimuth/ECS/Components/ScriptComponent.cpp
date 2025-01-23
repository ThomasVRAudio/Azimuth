#include <Azimuth/ECS/Components/ScriptComponent.h>

namespace Azimuth
{
    void ScriptComponent::OnStart()
    {
        for (auto &script : m_Scripts)
            script->OnStart();
    }

    void ScriptComponent::OnUpdate()
    {
        for (auto &script : m_Scripts)
            script->OnUpdate();
    }
}