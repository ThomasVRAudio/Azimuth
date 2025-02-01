#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorLayer.h>

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
        m_RenderSystem->Init(ECS, m_LightSystem);

        EditorUI::Init(scene);
        EditorUI::SetLightsUpdateCallback([&]()
                                          { m_LightSystem->UpdateLights(); });

        unsigned int width = static_cast<unsigned int>(m_EditorSceneTextureWidth);
        unsigned int height = static_cast<unsigned int>(m_EditorSceneTextureWidth * (9.0f / 16.0f));

        m_FrameBufferConfig = std::make_unique<FrameBufferConfig>(ColorAttachment{width, height, &m_EditorSceneTexture});
        FrameBuffer::CreateFramebuffer(m_FrameBufferConfig.get());

        m_EditorShader = std::make_shared<Shader>("assets/shaders/editor/unlit.vert", "assets/shaders/editor/unlit.frag");
        ColorAttachment entityIDAttachment{
            width,
            height,
            &m_EditorIDTexture,
            FrameBufferTextureFormat::RED_INTEGER,
            FrameBufferTextureFormat::RED_INTEGER_INTERNAL};

        m_FrameBufferEditorConfig = std::make_unique<FrameBufferConfig>(entityIDAttachment);
        FrameBuffer::CreateFramebuffer(m_FrameBufferEditorConfig.get());
    }

    void EditorLayer::OnStart() {}

    void EditorLayer::OnUpdate()
    {
        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);

        m_RenderSystem->RenderScene(m_EditorCamera, m_FrameBufferConfig->ID);
        m_RenderSystem->RenderEditorPass(m_EditorCamera, m_FrameBufferEditorConfig->ID, m_EditorShader.get(), m_EditorIDTexture);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        EditorUI::CreateDocker();
        EditorUI::DrawUI();
        EditorUI::DrawEditorScene(&m_EditorSceneTexture, m_EditorCamera);
        EditorUI::ReadPixelID(m_FrameBufferEditorConfig->ID, m_EditorSceneTextureWidth, m_EditorSceneTextureWidth * (9.0f / 16.0f), 0);
        EditorUI::EndDraw();

        m_EditorCamera.ProcessKeyboard();

        if (Input::IsKeyPressed(Escape))
            glfwSetInputMode(Window::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

#endif