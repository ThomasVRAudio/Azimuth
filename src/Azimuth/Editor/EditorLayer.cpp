#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Events/MouseEvents.h>

namespace Azimuth
{

    void EditorLayer::Init()
    {
        ECSManager &ECS = ECSManager::GetInstance();

        ComponentMask mask;
        mask.set(ECS.GetComponentBitType<TransformComponent>(), true);
        mask.set(ECS.GetComponentBitType<MeshComponent>(), true);
        m_RenderSystem = ECS.RegisterSystem<RenderSystem>();
        ECS.SetSystemComponentMask<RenderSystem>(mask);

        m_RenderSystem->Init();
        EditorUI::Init();

        glfwSetInputMode(Window::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void EditorLayer::OnStart()
    {
        FrameBuffer::CreateFramebuffer(&m_FrameBuffer, &m_EditorSceneTexture,
                                       m_EditorSceneTextureWidth, m_EditorSceneTextureWidth * (9.0f / 16.0f));
    }

    void EditorLayer::OnUpdate()
    {
        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);

        EditorUI::DrawToBuffer(&m_FrameBuffer, [&]()
                               { m_RenderSystem->DrawScene(m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetProjectionMatrix()); });

        EditorUI::CreateDocker();
        EditorUI::DrawEditorScene(&m_EditorSceneTexture);
        EditorUI::DrawUI();

        EditorUI::EndDraw();

        m_EditorCamera.ProcessKeyboard();

        if (Input::IsKeyPressed(Escape))
            glfwSetInputMode(Window::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

#endif