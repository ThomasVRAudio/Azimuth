#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/Entity.h>

namespace Azimuth
{
    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity entity) = 0;
    };

    template <typename T>
    class ComponentArray : public IComponentArray
    {
    public:
        void InsertData(Entity entity, T component)
        {
            assert(m_EntityToComponentIndexMap.find(entity) == m_EntityToComponentIndexMap.end() && "Component added to same entity more than once");

            size_t newIndex = m_ComponentArraySize;
            m_EntityToComponentIndexMap[entity] = newIndex;
            m_IndexToEntityMap[newIndex] = entity;
            m_ComponentArray[newIndex] = component;
            ++m_ComponentArraySize;
        }

        void RemoveData(Entity entity)
        {
            assert(m_EntityToComponentIndexMap.find(entity) != m_EntityToComponentIndexMap.end() && "Entity not in Component Array");

            /*
                - Retrieve the last entity.
                - Update the last entity's index to the removed entity's index.
                - Move the last entity to the removed entity's position.
                - Erase mappings for the removed entity and the last entity's old position.
            */

            size_t indexOfRemovedEntity = m_EntityToComponentIndexMap[entity];
            size_t indexOfLastElement = m_ComponentArraySize - 1;
            m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

            Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
            m_EntityToComponentIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            m_EntityToComponentIndexMap.erase(entity);
            m_IndexToEntityMap.erase(indexOfLastElement);

            --m_ComponentArraySize;
        }

        T &GetData(Entity entity)
        {
            assert(m_EntityToComponentIndexMap.find(entity) != m_EntityToComponentIndexMap.end() && "Entity not in Component Array");

            return m_ComponentArray[m_EntityToComponentIndexMap[entity]];
        }

        void EntityDestroyed(Entity entity) override
        {
            if (m_EntityToComponentIndexMap.find(entity) != m_EntityToComponentIndexMap.end())
            {
                RemoveData(entity);
            }
        }

    private:
        std::array<T, MAX_ENTITIES> m_ComponentArray;
        std::unordered_map<Entity, size_t> m_EntityToComponentIndexMap;
        std::unordered_map<size_t, Entity> m_IndexToEntityMap;
        size_t m_ComponentArraySize;
    };

}