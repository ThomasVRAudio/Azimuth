#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Events/MouseEvents.h>

namespace Azimuth
{

    void EditorLayer::Init(Scene *scene)
    {
        ECSManager *ECS = scene->GetECSManager();
        ComponentMask mask;

        mask.set(ECS->GetComponentBitType<TransformComponent>(), true);
        mask.set(ECS->GetComponentBitType<MeshComponent>(), true);
        m_RenderSystem = ECS->RegisterSystem<RenderSystem>();
        ECS->SetSystemComponentMask<RenderSystem>(mask);

        mask.reset();
        mask.set(ECS->GetComponentBitType<LightComponent>(), true);
        mask.set(ECS->GetComponentBitType<TransformComponent>(), true);
        m_LightSystem = ECS->RegisterSystem<LightSystem>();

        ECS->SetSystemComponentMask<LightSystem>(mask);

        m_LightSystem->Init(ECS);
        m_RenderSystem->Init(ECS, m_LightSystem, m_EditorSceneTextureWidth,
                             m_EditorSceneTextureWidth * (9.0f / 16.0f), &m_EditorSceneTexture);

        EditorUI::Init(scene);
        EditorUI::SetLightsUpdateCallback([&]()
                                          { m_LightSystem->UpdateLights(); });
    }

    void EditorLayer::OnStart() {}

    void EditorLayer::OnUpdate()
    {
        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);

        m_RenderSystem->RenderScene(m_EditorCamera);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        EditorUI::CreateDocker();
        EditorUI::DrawUI();
        EditorUI::DrawEditorScene(&m_EditorSceneTexture, m_EditorCamera);
        EditorUI::EndDraw();

        m_EditorCamera.ProcessKeyboard();

        if (Input::IsKeyPressed(Escape))
            glfwSetInputMode(Window::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

#endif