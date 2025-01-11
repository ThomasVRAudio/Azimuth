#include <Azimuth/ECS/Component.h>

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