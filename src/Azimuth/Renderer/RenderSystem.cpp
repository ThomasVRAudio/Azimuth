#include <Azimuth/Renderer/RenderSystem.h>
#include <dependencies/GLFW/glfw3.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{

    void RenderSystem::Init()
    {
        // Creating Entities shouldn't be the render system but we're testing so who cares. Maybe create a game layer
        ECSManager &ECS = ECSManager::GetInstance();
        // Entity entity = ECS.CreateEntity();
        // TransformComponent component;
        // ECS.AddComponent<TransformComponent>(entity, component);
    }

    void RenderSystem::DrawScene()
    {
        ECSManager &ECS = ECSManager::GetInstance();
        for (auto &entity : m_Entities)
        {
            auto transform = ECS.GetComponent<TransformComponent>(entity).Position;
            transform = glm::vec3(glm::sin(glfwGetTime()), 0.0f, 0.0f);
            // print("entity: " << entity << " x: " << transform.x);
        }
    };
}