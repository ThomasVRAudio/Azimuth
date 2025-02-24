#pragma once
#include <Azimuth/ECS/ComponentArray.h>
#include <typeindex>

namespace Azimuth
{

    class ComponentManager
    {
    public:
        template <typename T>
        void RegisterComponent()
        {

            std::type_index typeIndex(typeid(T));

            assert(m_ComponentBitTypes.find(typeIndex) == m_ComponentBitTypes.end() && "Registering component type more than once");

            m_ComponentBitTypes.insert({typeIndex, m_NextComponentBitType});
            m_ComponentArrays.insert({typeIndex, std::make_shared<ComponentArray<T>>()});

            ++m_NextComponentBitType;
        }

        template <typename T>
        ComponentBitType GetComponentBitType()
        {
            std::type_index typeIndex(typeid(T));

            assert(m_ComponentBitTypes.find(typeIndex) != m_ComponentBitTypes.end() && "Component not registered before use");

            return m_ComponentBitTypes[typeIndex];
        }

        template <typename T>
        void AddComponent(Entity entity, T component)
        {
            GetComponentArray<T>()->InsertData(entity, component);
        }

        template <typename T>
        void RemoveComponent(Entity entity)
        {
            GetComponentArray<T>()->RemoveData(entity);
        }

        template <typename T>
        T &GetComponent(Entity entity)
        {
            return GetComponentArray<T>()->GetData(entity);
        }

        template <typename T>
        bool HasComponent(Entity entity)
        {
            return GetComponentArray<T>()->HasData(entity);
        }

        void EntityDestroyed(Entity entity)
        {
            for (auto const &[_, component] : m_ComponentArrays)
            {
                component->EntityDestroyed(entity);
            }
        }

    private:
        std::unordered_map<std::type_index, ComponentBitType> m_ComponentBitTypes{};
        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
        ComponentBitType m_NextComponentBitType{};

        template <typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            std::type_index typeIndex(typeid(T));
            assert(m_ComponentBitTypes.find(typeIndex) != m_ComponentBitTypes.end() && "Component not registered before use.");

            return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeIndex]);
        }
    };
}