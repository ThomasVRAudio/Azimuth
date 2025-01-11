#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/Entity.h>
#include <Azimuth/ECS/System.h>

namespace Azimuth
{
    class SystemManager
    {

    public:
        template <typename T>
        typename std::enable_if<std::is_base_of<System, T>::value, std::shared_ptr<T>>::type
        RegisterSystem()
        {
            const char *typeName = typeid(T).name();

            assert(m_Systems.find(typeName) == m_Systems.end() && "Registering system more than once.");

            std::shared_ptr<T> system = std::make_shared<T>();
            m_Systems.insert({typeName, system});
            return system;
        }

        template <typename T>
        typename std::enable_if<std::is_base_of<System, T>::value, void>::type
        SetComponentMask(ComponentMask componentMask)
        {
            const char *typeName = typeid(T).name();

            assert(m_Systems.find(typeName) != m_Systems.end() && "System not registered before use.");

            m_ComponentMasks.insert({typeName, componentMask});
        }

        void EntityDestroyed(Entity entity)
        {
            for (auto const &[_, system] : m_Systems)
            {
                system->m_Entities.erase(entity);
            }
        }

        void EntityComponentMaskChanged(Entity entity, ComponentMask entityComponentMask)
        {
            print("mask changed for entity: " << entity);
            for (auto const &[typeName, system] : m_Systems)
            {
                auto const &systemComponentMask = m_ComponentMasks[typeName];
                print("mask changed: " << entityComponentMask << "vs\n"
                                       << systemComponentMask)

                    // bit comparison, e.g. entity= 010101 and system= 00100, would become 00100
                    if ((entityComponentMask & systemComponentMask) == systemComponentMask)
                {
                    print("inserted");
                    system->m_Entities.insert(entity);
                }
                else
                {
                    print("erased");
                    system->m_Entities.erase(entity);
                }
            }
        }

    private:
        std::unordered_map<const char *, ComponentMask> m_ComponentMasks{};
        std::unordered_map<const char *, std::shared_ptr<System>> m_Systems{};
    };
}