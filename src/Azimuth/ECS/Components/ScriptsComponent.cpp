#include <Azimuth/ECS/Components/ScriptsComponent.h>

namespace Azimuth
{
    void ScriptsComponent::OnStart()
    {
        for (auto &script : m_Scripts)
            script->OnStart();
    }

    void ScriptsComponent::OnUpdate()
    {
        for (auto &script : m_Scripts)
            script->OnUpdate();
    }
}