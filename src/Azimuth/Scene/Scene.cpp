#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Renderer/RenderSystem.h>

namespace Azimuth
{
    Scene::Scene()
    {
        ECS.Init();
        ECS.RegisterComponent<TransformComponent>();
        ECS.RegisterComponent<MeshComponent>();
        ECS.RegisterComponent<AudioComponent>();

        ECS.RegisterSystem<RenderSystem>();

        ComponentMask mask;
        mask.set(ECS.GetComponentBitType<TransformComponent>(), true);
        ECS.SetSystemComponentMask<RenderSystem>(mask);

        Entity entity = ECS.CreateEntity();
        TransformComponent transform;
        transform.Position = glm::vec3(0.1f, 0.2f, 0.3f);
        ECS.AddComponent<TransformComponent>(entity, transform);

        ECS.GetComponent<TransformComponent>(entity).Position = glm::vec3(0.1f, 0.1f, 0.5f);

        std::cout << ECS.GetComponent<TransformComponent>(entity).Position.z << std::endl;

        // std::cout << ECS.GetComponent<TransformComponent>(entity).Position << std::endl;
    };
}