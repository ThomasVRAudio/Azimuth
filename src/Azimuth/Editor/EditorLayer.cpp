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
    }

    void EditorLayer::OnStart()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiStyling::SetStyling();
        ImGui_ImplGlfw_InitForOpenGL(Window::m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        m_WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        m_WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        m_WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiConfigFlags_ViewportsEnable;

        FrameBuffer::CreateFramebuffer(&frameBuffer, &texture, width, height);

        ImFontConfig fontConfig;
        fontConfig.OversampleH = 4;
        fontConfig.OversampleV = 4;
        fontConfig.PixelSnapH = false;
        io.Fonts->AddFontFromFileTTF("assets/fonts/Open_Sans/OpenSans-SemiBold.ttf", 18.0f, &fontConfig);
    }

    void EditorLayer::OnUpdate()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glViewport(0, 0, width, height);

        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_RenderSystem->DrawScene();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::Begin("DockSpace", nullptr, m_WindowFlags);

        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        float left_padding = 10.0f;
        ImGui::End();
        ImGui::Begin("Hierarchy");
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Indent(left_padding);
        ImGui::Text("Entities");
        ImGui::Text("Components");
        ImGui::Unindent(left_padding);
        ImGui::End();

        ImGui::Begin("Properties");
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Indent(left_padding);
        ImGui::Text("Transform");
        ImGui::Separator();
        ImGui::Text("Mesh");
        ImGui::Unindent(left_padding);
        ImGui::End();

        ImGui::Begin("Settings");
        static bool gizmos;
        ImGui::Indent(left_padding);
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Checkbox("Gizmos", &gizmos);
        ImGui::ColorEdit4("Solid BG Color", (float *)&m_ClearColor);
        ImGui::Unindent(left_padding);
        ImGui::End();

        ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        ImVec2 size = ImGui::GetContentRegionAvail();
        ImGui::Image((ImTextureID)(intptr_t)texture, size, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        ImGui::Begin("Logs");
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Indent(left_padding);
        ImGui::Text("Right click!");
        ImGui::Text("Loaded entity");
        ImGui::Text("Printing ..");
        ImGui::Unindent(left_padding);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}