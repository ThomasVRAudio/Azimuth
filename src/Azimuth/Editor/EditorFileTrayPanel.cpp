#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorFileTrayPanel.h>
#include <Azimuth/Editor/EditorTextureLoader.h>
#include <Azimuth/Project/Serializer.h>
#include <Azimuth/Project/FileGenerator.h>
#include <Azimuth/Editor/EditorManager.h>

namespace Azimuth
{

    void EditorFileTrayPanel::Init()
    {
        m_AssetPath = Application::projectSettings->ProjectFolder;
        m_CurrentPath = m_AssetPath;
    }

    void EditorFileTrayPanel::DrawPanel()
    {
        float left_padding = 10.0f;
        ImGui::Begin("FileTray");
        ImGui::Indent(left_padding);

        ImGui::Dummy(ImVec2(4.0f, 4.0f));

        if (m_CurrentPath != m_AssetPath)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

            if (ImGui::ImageButton("back_button", EditorTextureLoader::GetTextureID("back"), ImVec2(60.0f, 36.0f)))
                m_CurrentPath = m_CurrentPath.parent_path();

            ImGui::PopStyleColor();
        }

        ImGui::SameLine();
        ImGui::SeparatorText(m_CurrentPath.filename().stem().string().c_str());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

        ImVec2 buttonSize = ImVec2(100, 100);
        ImVec2 windowSize = ImGui::GetWindowSize();
        int columns = glm::floor(windowSize.x / buttonSize.x);

        ImGui::Columns(columns, nullptr, false);

        ImVec2 image_padding(2.0f, 2.0f);

        if (ImGui::Button("Create Scripts"))
        {
            FileGenerator::GenerateScripts(m_CurrentPath, "Test");
        }

        for (const auto &file : std::filesystem::directory_iterator(m_CurrentPath))
        {
            if ((file.is_directory() && file.path().filename().string()[0] == '.') || file.path().filename().string() == "build")
                continue;

            if (file.is_directory())
            {
                ImGui::BeginGroup();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
                if (ImGui::ImageButton(file.path().string().c_str(), EditorTextureLoader::GetTextureID("folder"), buttonSize))
                {
                    m_CurrentPath = file.path();
                }
                ImGui::PopStyleVar();
                ImGui::PopStyleColor();

                std::string label = file.path().stem().string();

                float textWidth = ImGui::CalcTextSize(label.c_str()).x;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (buttonSize.x - textWidth) * 0.5f);
                ImGui::Text("%s", label.c_str());

                ImGui::EndGroup();
            }
            else
            {
                ImGui::BeginGroup();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.8f));

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, image_padding);

                unsigned int texture;

                if (file.path().extension() == ".jpg" || file.path().extension() == ".png")
                    texture = EditorTextureLoader::GetOrLoadTexture(file.path().string());
                else
                    texture = EditorTextureLoader::GetTextureID("file");

                if (ImGui::ImageButton(file.path().string().c_str(), texture, buttonSize - image_padding))
                {

                    if (file.path().extension() == ".scene")
                    {
                        EditorManager::m_SelectedEntity = -1;
                        std::string newScenePath;
                        Serializer::OpenScene(EditorManager::m_Scene, file.path().string(), &newScenePath);
                        Application::projectSettings->MainScenePath = std::filesystem::path(newScenePath);
                    }

                    if (std::filesystem::exists(file.path()) && (file.path().extension() == ".cpp" || file.path().extension() == ".h"))
                    {
                        std::string command = "code \"" + file.path().string() + "\"";
                        int result = system(command.c_str());
                    }
                }

                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("file", file.path().string().c_str(), file.path().string().size() + 1);
                    ImGui::Text("%s", file.path().filename().string().c_str());
                    ImGui::EndDragDropSource();
                }

                ImGui::PopStyleVar();
                ImGui::PopStyleColor(2);

                std::string label = file.path().filename().string();

                ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + buttonSize.x);
                ImGui::Text("%s", label.c_str());
                ImGui::PopTextWrapPos();

                ImGui::EndGroup();
            }

            ImGui::NextColumn();
        }

        ImGui::Columns(1);
        ImGui::PopStyleVar();

        ImGui::Unindent(left_padding);
        ImGui::End();
    }
}

#endif