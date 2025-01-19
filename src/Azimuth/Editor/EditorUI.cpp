#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorUI.h>

namespace Azimuth
{
    ImGuiIO *EditorUI::io = nullptr;
    ImGuiWindowFlags EditorUI::m_WindowFlags;
    ImVec2 EditorUI::m_SceneWindowPos;
    ImVec2 EditorUI::m_SceneWindowSize;

    void EditorUI::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiStyling::SetStyling();

        bool success = ImGui_ImplGlfw_InitForOpenGL(Window::GetMainWindow(), true);
        assert(success && "ImGui_ImplGlfw_InitForOpenGL failed!");

        success = ImGui_ImplOpenGL3_Init("#version 330");
        assert(success && "ImGui_ImplOpenGL3_Init failed!");

        io = &ImGui::GetIO();

        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        m_WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        m_WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        m_WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiConfigFlags_ViewportsEnable;

        ImFontConfig fontConfig;
        fontConfig.OversampleH = 4;
        fontConfig.OversampleV = 4;
        fontConfig.PixelSnapH = false;
        io->Fonts->AddFontFromFileTTF("assets/fonts/Open_Sans/OpenSans-SemiBold.ttf", 18.0f, &fontConfig);
    }

    void EditorUI::DrawUI()
    {

        float left_padding = 10.0f;
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
        ImGui::Indent(left_padding);
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Checkbox("Play Scene", &Application::s_PlayingEditorScene);
        ImGui::Unindent(left_padding);
        ImGui::End();

        ImGui::Begin("Logs");
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Indent(left_padding);
        ImGui::Text("Right click!");
        ImGui::Text("Loaded entity");
        ImGui::Text("Printing ..");
        ImGui::Unindent(left_padding);
        ImGui::End();
    }

    void EditorUI::DrawEditorScene(unsigned int *texture)
    {
        ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        m_SceneWindowSize = ImGui::GetContentRegionAvail();
        m_SceneWindowPos = ImGui::GetWindowPos();

        ImGui::Image((ImTextureID)(*texture), m_SceneWindowSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }

    void EditorUI::EndDraw()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void EditorUI::CreateDocker()
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::Begin("DockSpace", nullptr, m_WindowFlags);
        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        ImGui::End();
    }

}
#endif