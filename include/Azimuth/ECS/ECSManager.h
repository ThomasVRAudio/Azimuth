#pragma once
#include <Azimuth/ECS/SystemManager.h>
#include <Azimuth/ECS/ComponentManager.h>
#include <Azimuth/ECS/EntityManager.h>
#include <Azimuth/ECS/Entity.h>

namespace Azimuth
{
    class ECSManager
    {
    public:
        void Init()
        {
            m_ComponentManager = std::make_unique<ComponentManager>();
            m_EntityManager = std::make_unique<EntityManager>();
            m_SystemManager = std::make_unique<SystemManager>();
        }

        Entity CreateEntity()
        {
            return m_EntityManager->CreateEntity();
        }

        void DestroyEntity(Entity entity)
        {
            m_EntityManager->DestroyEntity(entity);
            m_SystemManager->EntityDestroyed(entity);
            m_ComponentManager->EntityDestroyed(entity);
        }

        template <typename T>
        void RegisterComponent()
        {
            m_ComponentManager->RegisterComponent<T>();
        }

        template <typename T>
        void AddComponent(Entity entity, T component)
        {
            m_ComponentManager->AddComponent<T>(entity, component);

            ComponentMask componentMask = m_EntityManager->GetComponentMask(entity);
            componentMask.set(m_ComponentManager->GetComponentBitType<T>(), true); // set a bit 0 to 1
            m_EntityManager->SetComponentMask(entity, componentMask);

            m_SystemManager->EntityComponentMaskChanged(entity, componentMask);
        }

        template <typename T>
        void RemoveComponent(Entity entity)
        {
            m_ComponentManager->RemoveComponent<T>(entity);

            ComponentMask componentMask = m_EntityManager->GetComponentMask(entity);
            componentMask.set(m_ComponentManager->GetComponentBitType<T>(), false);
            m_EntityManager->SetComponentMask(entity, componentMask);

            m_SystemManager->EntityComponentMaskChanged(entity, componentMask);
        }

        template <typename T>
        T &GetComponent(Entity entity)
        {
            return m_ComponentManager->GetComponent<T>(entity);
        }

        template <typename T>
        bool HasComponent(Entity entity)
        {
            return m_ComponentManager->HasComponent<T>(entity);
        }

        template <typename T>
        ComponentBitType GetComponentBitType()
        {
            return m_ComponentManager->GetComponentBitType<T>();
        }

        template <typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            return m_SystemManager->RegisterSystem<T>();
        }

        template <typename T>
        typename std::enable_if<std::is_base_of<System, T>::value, void>::type
        SetSystemComponentMask(ComponentMask componentMask)
        {
            m_SystemManager->SetComponentMask<T>(componentMask);
        }

        ECSManager(const ECSManager &) = delete;
        ECSManager &operator=(const ECSManager &) = delete;

        static ECSManager &GetInstance()
        {
            static ECSManager instance;
            return instance;
        }

    private:
        std::unique_ptr<ComponentManager> m_ComponentManager;
        std::unique_ptr<EntityManager> m_EntityManager;
        std::unique_ptr<SystemManager> m_SystemManager;
        ECSManager() {}
    };

}