#include "Azimuth/ECS/EntityManager.h"

namespace Azimuth
{
    EntityManager::EntityManager()
    {
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        {
            m_AvailableEntitiesPool.push(entity);
        }
    }

    Entity EntityManager::CreateEntity()
    {
        assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

        Entity id = m_AvailableEntitiesPool.front();
        m_AvailableEntitiesPool.pop();
        ++m_LivingEntityCount;

        return id;
    }

    void EntityManager::DestroyEntity(Entity entity)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        m_ComponentMasks[entity].reset(); // set bits to 0000 ..
        m_AvailableEntitiesPool.push(entity);
        --m_LivingEntityCount;
    }

    void EntityManager::SetComponentMask(Entity entity, ComponentMask componentMask)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        m_ComponentMasks[entity] = componentMask;
    }

    ComponentMask EntityManager::GetComponentMask(Entity entity)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        return m_ComponentMasks[entity];
    };
}