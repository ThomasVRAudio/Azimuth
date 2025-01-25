#include <Azimuth/Game/GameModeLayer.h>
#include <Azimuth/ECS/Components/ScriptComponent.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Events/MouseEvents.h>

namespace Azimuth
{
    void GameModeLayer::Init(Scene *scene)
    {
        ECSManager *ECS = scene->GetECSManager();

        ComponentMask mask;
        mask.set(ECS->GetComponentBitType<TransformComponent>(), true);
        mask.set(ECS->GetComponentBitType<MeshComponent>(), true);
        m_RenderSystem = ECS->RegisterSystem<RenderSystem>();
        ECS->SetSystemComponentMask<RenderSystem>(mask);

        // m_RenderSystem->Init(ECS);

        glfwSetInputMode(Window::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_MainCamera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
    }

    void GameModeLayer::OnStart()
    {
    }

    void GameModeLayer::OnUpdate()
    {
        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);

        // m_RenderSystem->DrawScene(m_MainCamera.GetViewMatrix(), m_MainCamera.GetProjectionMatrix());

        if (Input::IsKeyPressed(Escape))
            glfwSetInputMode(Window::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}