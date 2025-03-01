#include <Azimuth/ECS/ECSManager.h>

namespace Azimuth
{
    ECSManager::ECSManager(const ECSManager &other)
    {
        m_ComponentManager = std::make_unique<ComponentManager>(*other.m_ComponentManager);
        m_EntityManager = other.m_EntityManager;
        m_SystemManager = other.m_SystemManager;
    }
}