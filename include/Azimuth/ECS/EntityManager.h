#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/Entity.h>

namespace Azimuth
{

    class EntityManager
    {
    public:
        EntityManager();
        ~EntityManager() = default;
        Entity CreateEntity();
        void DestroyEntity(Entity entity);
        void SetComponentMask(Entity entity, ComponentMask componentMask);
        ComponentMask GetComponentMask(Entity entity);

    private:
        std::queue<Entity> m_AvailableEntitiesPool{};
        std::array<ComponentMask, MAX_ENTITIES> m_ComponentMasks{};
        uint32_t m_LivingEntityCount = 0;
    };

}