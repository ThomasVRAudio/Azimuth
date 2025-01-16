#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/ECS/Component.h>

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
                               { m_RenderSystem->DrawScene(); });

        EditorUI::CreateDocker();
        EditorUI::DrawEditorScene(&m_EditorSceneTexture);
        EditorUI::DrawUI();

        EditorUI::EndDraw();
    }
}