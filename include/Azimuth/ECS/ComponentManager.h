#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/ComponentArray.h>

namespace Azimuth
{

    class ComponentManager
    {
    public:
        template <typename T>
        void RegisterComponent()
        {
            const char *typeName = typeid(T).name(); // unique

            assert(m_ComponentBitTypes.find(typeName) == m_ComponentBitTypes.end() && "Registering component type more than once");

            m_ComponentBitTypes.insert({typeName, m_NextComponentBitType});
            m_ComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            ++m_NextComponentBitType;
        }

        template <typename T>
        ComponentBitType GetComponentBitType()
        {
            const char *typeName = typeid(T).name();

            assert(m_ComponentBitTypes.find(typeName) != m_ComponentBitTypes.end() && "Component not registered before use");

            return m_ComponentBitTypes[typeName];
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

        void EntityDestroyed(Entity entity)
        {
            for (auto const &[_, component] : m_ComponentArrays)
            {
                component->EntityDestroyed(entity);
            }
        }

    private:
        std::unordered_map<const char *, ComponentBitType> m_ComponentBitTypes{};
        std::unordered_map<const char *, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
        ComponentBitType m_NextComponentBitType{};

        template <typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            const char *typeName = typeid(T).name();

            assert(m_ComponentBitTypes.find(typeName) != m_ComponentBitTypes.end() && "Component not registered before use.");

            return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
        }
    };
}