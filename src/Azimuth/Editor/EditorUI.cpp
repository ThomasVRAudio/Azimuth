#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorUI.h>

namespace Azimuth
{
    void EditorUI::Init(Scene *scene)
    {
        m_Scene = scene;
        m_SceneSettings = scene->Settings;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiStyling::SetStyling();

        bool success = ImGui_ImplGlfw_InitForOpenGL(Window::GetMainWindow(), true);
        assert(success && "ImGui_ImplGlfw_InitForOpenGL failed!");

        success = ImGui_ImplOpenGL3_Init();
        assert(success && "ImGui_ImplOpenGL3_Init failed!");

        ImGuiIO &io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        m_WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        m_WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        m_WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiConfigFlags_ViewportsEnable;

        ImFontConfig fontConfig;
        fontConfig.OversampleH = 4;
        fontConfig.OversampleV = 4;
        fontConfig.PixelSnapH = false;
        io.Fonts->AddFontFromFileTTF("assets/fonts/Open_Sans/OpenSans-SemiBold.ttf", 18.0f, &fontConfig);
    }

    void EditorUI::DrawUI()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        float left_padding = 10.0f;

        if (!t_HasOpenedScene)
        {
            Serializer::OpenScene(m_Scene, "D:/Users/Thomas/Documents/Dev/Engine/build/Scenes/Basic.scene");
            t_HasOpenedScene = true;
        }

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open"))
                {
                    m_SelectedEntity = -1;
                    Serializer::OpenScene(m_Scene);
                }
                if (ImGui::MenuItem("Save As..."))
                {
                    Serializer::SaveScene(m_Scene);
                }
                if (ImGui::MenuItem("Quit"))
                {
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
        ImGui::PopStyleVar(2);

        EditorHierarchyPanel::DrawPanel();
        EditorPropertiesPanel::DrawPanel();
        EditorSettingsPanel::DrawPanel();

        ImGui::Begin("Logs");
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Indent(left_padding);
        ImGui::Text("Right click!");
        ImGui::Text("Loaded entity");
        ImGui::Text("Printing ..");
        ImGui::Unindent(left_padding);
        ImGui::End();
    }

    void EditorUI::DrawGizmos(Camera &camera)
    {
        if (m_SelectedEntity < 0)
            return;

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x + m_SceneWindowPadding.x, ImGui::GetWindowPos().y + m_SceneWindowPadding.y, m_SceneWindowSize.x, m_SceneWindowSize.y);

        TransformComponent &transform = m_Scene->GetComponent<TransformComponent>(m_SelectedEntity);
        glm::mat4 transformMatrix = transform.GetTransform();

        if (Input::IsKeyPressed(W))
            gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        if (Input::IsKeyPressed(Q))
            gizmoOperation = ImGuizmo::OPERATION::ROTATE;
        if (Input::IsKeyPressed(E))
            gizmoOperation = ImGuizmo::OPERATION::SCALE;

        ImGuizmo::Manipulate(glm::value_ptr(camera.GetViewMatrix()), glm::value_ptr(camera.GetProjectionMatrix()),
                             gizmoOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr(transformMatrix));

        if (ImGuizmo::IsUsing())
        {
            m_IsManipulating = true;
            glm::vec3 translation, rotation, scale;

            glm::mat4 LocalMatrix(transformMatrix);

            if (
                glm::epsilonNotEqual(LocalMatrix[0][3], static_cast<float>(0), glm::epsilon<float>()) ||
                glm::epsilonNotEqual(LocalMatrix[1][3], static_cast<float>(0), glm::epsilon<float>()) ||
                glm::epsilonNotEqual(LocalMatrix[2][3], static_cast<float>(0), glm::epsilon<float>()))
            {
                LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<float>(0);
                LocalMatrix[3][3] = static_cast<float>(1);
            }

            translation = glm::vec3(LocalMatrix[3]);
            LocalMatrix[3] = glm::vec4(0, 0, 0, LocalMatrix[3].w);

            glm::vec3 Row[3];

            for (glm::length_t i = 0; i < 3; ++i)
                for (glm::length_t j = 0; j < 3; ++j)
                    Row[i][j] = LocalMatrix[i][j];

            scale.x = length(Row[0]);
            scale.y = length(Row[1]);
            scale.z = length(Row[2]);

            Row[0] = glm::normalize(Row[0]);
            Row[1] = glm::normalize(Row[1]);
            Row[2] = glm::normalize(Row[2]);

            rotation.y = asin(-Row[0][2]);
            if (cos(rotation.y) != 0)
            {
                rotation.x = atan2(Row[1][2], Row[2][2]);
                rotation.z = atan2(Row[0][1], Row[0][0]);
            }
            else
            {
                rotation.x = atan2(-Row[2][0], Row[1][1]);
                rotation.z = 0;
            }

            transform.Position = translation;
            transform.Rotation = rotation;
            transform.Scale = scale;
        }
        else
        {
            m_IsManipulating = false;
        }
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

    void EditorUI::DrawEditorScene(unsigned int *texture, Camera &camera)
    {

        ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX), SetAspectConstraints);

        ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        m_SceneWindowSize = ImGui::GetContentRegionAvail();
        m_SceneWindowPos = ImGui::GetWindowPos() + ImGui::GetCursorPos();
        ImVec2 availableSize = m_SceneWindowSize;

        if (m_SceneWindowSize.x / m_SceneWindowSize.y > m_SceneWindowAspectRatio)
            m_SceneWindowSize.x = m_SceneWindowSize.y * m_SceneWindowAspectRatio;
        else
            m_SceneWindowSize.y = m_SceneWindowSize.x / m_SceneWindowAspectRatio;

        m_SceneWindowPadding = ImGui::GetCursorPos();

        ImGui::Image((ImTextureID)(*texture), m_SceneWindowSize, ImVec2(0, 1), ImVec2(1, 0));

        DrawGizmos(camera);
        ImGui::End();
    }

    void EditorUI::ReadPixelID(unsigned int frameBufferID, unsigned int textureWidth, unsigned int textureHeight, unsigned int attachment)
    {

        if (!Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            return;

        double mouseX, mouseY;
        glfwGetCursorPos(Window::GetMainWindow(), &mouseX, &mouseY);

        double x = mouseX - m_SceneWindowPos.x;
        double y = m_SceneWindowSize.y - (mouseY - m_SceneWindowPos.y);

        double normalizedX = x / m_SceneWindowSize.x;
        double normalizedY = y / m_SceneWindowSize.y;

        if (x <= 0.0f || y <= 0.0f || x >= m_SceneWindowSize.x || y >= m_SceneWindowSize.y)
            return;

        int texX = static_cast<int>(normalizedX * textureWidth);
        int texY = static_cast<int>(normalizedY * textureHeight);

        int pixelData;
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
        glReadPixels(texX, texY, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        GLenum error;
        error = glGetError();
        if (error != GL_NO_ERROR)
            print("ReadPixel ID Error: " << error);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (!m_IsManipulating)
            m_SelectedEntity = pixelData;
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