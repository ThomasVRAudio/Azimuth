#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorUI.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    void EditorUI::Init(Scene *scene)
    {
        m_Scene = scene;
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
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        float left_padding = 10.0f;
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    // Handle Save logic
                }
                if (ImGui::MenuItem("Save As..."))
                {
                    // Handle Save As logic
                }
                if (ImGui::MenuItem("Quit"))
                {
                    // Handle Quit logic
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
        ImGui::PopStyleVar(2);

        EditorHierarchyPanel::DrawPanel();
        EditorPropertiesPanel::DrawPanel();

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

    void EditorUI::SetAspectConstraints(ImGuiSizeCallbackData *data)
    {
        float width = data->CurrentSize.x;
        float height = data->CurrentSize.y;

        if (width / height > m_SceneWindowAspectRatio)
            width = height * m_SceneWindowAspectRatio;
        else
            height = width / m_SceneWindowAspectRatio;

        data->DesiredSize = ImVec2(width, height);
    }

    void EditorUI::DrawEditorScene(unsigned int *texture)
    {

        ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX), SetAspectConstraints);

        ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        m_SceneWindowSize = ImGui::GetContentRegionAvail();
        ImVec2 availableSize = m_SceneWindowSize;

        if (m_SceneWindowSize.x / m_SceneWindowSize.y > m_SceneWindowAspectRatio)
            m_SceneWindowSize.x = m_SceneWindowSize.y * m_SceneWindowAspectRatio;
        else
            m_SceneWindowSize.y = m_SceneWindowSize.x / m_SceneWindowAspectRatio;

        ImVec2 padding((availableSize.x - m_SceneWindowSize.x) * 0.5f, (availableSize.y - m_SceneWindowSize.y) * 0.5f);
        ImGui::SetCursorPos(ImGui::GetCursorPos() + padding);

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