#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorFileTrayPanel.h>
#include <Azimuth/Editor/EditorTextureLoader.h>
#include <Azimuth/Editor/EditorPlayState.h>
#include <Azimuth/Project/Serializer.h>
#include <Azimuth/Project/FileGenerator.h>
#include <Azimuth/Editor/EditorManager.h>
#include <Azimuth/Core/Time.h>

namespace Azimuth
{

    void EditorFileTrayPanel::Init()
    {
        m_AssetPath = Application::projectSettings->ProjectFolder;
        m_CurrentPath = m_AssetPath;
    }

    void EditorFileTrayPanel::ClearPopup()
    {
        m_OnTextSubmitFunc = nullptr;
        m_InputText[0] = '\0';
        ImGui::CloseCurrentPopup();
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

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        if (ImGui::BeginPopupContextWindow("Popup", ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("New file"))
            {
                m_PopupTitle = "New File Name:";
                m_OnTextSubmitFunc = []()
                {
                    if (std::filesystem::exists(m_CurrentPath / m_InputText))
                    {
                        std::cout << "File already exists!" << std::endl;
                        return;
                    }

                    std::ofstream file(m_CurrentPath / m_InputText);

                    if (file)
                    {
                        std::cout << "File created successfully!" << std::endl;
                        file.close();
                    }
                    else
                    {
                        std::cout << "Failed to create the file!" << std::endl;
                    }
                };
            }
            if (ImGui::MenuItem("New folder"))
            {
                m_PopupTitle = "New Folder Name:";
                m_OnTextSubmitFunc = []()
                { std::filesystem::create_directory(m_CurrentPath / m_InputText); };
            }
            if (ImGui::MenuItem("New script"))
            {
                m_PopupTitle = "New Script Name:";
                m_OnTextSubmitFunc = []()
                { FileGenerator::GenerateScripts(m_CurrentPath, m_InputText); };
            }
            if (ImGui::MenuItem("New shader"))
            {
                m_PopupTitle = "New Shader Name:";
                m_OnTextSubmitFunc = []()
                { FileGenerator::GenerateGLSLFile(m_CurrentPath, m_InputText); };
            }
            if (ImGui::MenuItem("Open in file explorer"))
            {
                ShellExecute(NULL, "open", m_CurrentPath.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
            }
            ImGui::EndMenu();
        }
        ImGui::PopStyleVar(2);

        if (m_OnTextSubmitFunc != nullptr)
            ImGui::OpenPopup("TextInputPopup");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        if (ImGui::BeginPopup("TextInputPopup"))
        {
            if (ImGui::IsKeyPressed(ImGuiKey_Escape))
                ClearPopup();

            if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
                ClearPopup();

            ImGui::PushItemWidth(200.0f);
            ImGui::Text(m_PopupTitle.c_str());
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::SetKeyboardFocusHere();
            if (ImGui::InputText("##input", m_InputText, IM_ARRAYSIZE(m_InputText), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (m_OnTextSubmitFunc)
                {
                    m_OnTextSubmitFunc();
                    ClearPopup();
                }
            }
            ImGui::PopItemWidth();
            ImGui::EndPopup();
        }
        ImGui::PopStyleVar(2);

        m_Time += Time::DeltaTime();
        if ((EditorPlayState::GetPlayState() != PlayState::PLAYING && m_Time >= 1.0f) ||
            m_PreviousPath != m_CurrentPath)
        {
            m_PreviousPath = m_CurrentPath;
            m_Time = 0.0f;

            m_Files.clear();
            m_Folders.clear();
            for (const auto &file : std::filesystem::directory_iterator(m_CurrentPath))
            {
                if (file.is_directory())
                {
                    if (std::find(m_HiddenFolders.begin(), m_HiddenFolders.end(), file.path().filename().string()) != m_HiddenFolders.end())
                        continue;

                    m_Folders.emplace_back(file);
                }
                else
                {
                    if (file.path().filename() != "CMakeLists.txt")
                        m_Files.emplace_back(file);
                }
            }
        }

        for (const auto &folder : m_Folders)
        {
            ImGui::BeginGroup();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
            if (ImGui::ImageButton(folder.path().string().c_str(), EditorTextureLoader::GetTextureID("folder"), buttonSize))
            {
                m_CurrentPath = folder.path();
            }
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("file"))
                {
                    const char *droppedFilePath = static_cast<const char *>(payload->Data);
                    std::filesystem::path source(droppedFilePath);
                    std::filesystem::rename(source, folder.path() / source.filename());
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();

            std::string label = folder.path().stem().string();

            float textWidth = ImGui::CalcTextSize(label.c_str()).x;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (buttonSize.x - textWidth) * 0.5f);
            ImGui::Text("%s", label.c_str());

            ImGui::EndGroup();
            ImGui::NextColumn();
        }

        for (const auto &file : m_Files)
        {
            if (file.path().filename().string()[0] == '.' || file.path().filename().string() == "build")
                continue;

            ImGui::BeginGroup();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.8f));

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, image_padding);

            unsigned int texture;

            if (file.path().extension() == ".jpg" || file.path().extension() == ".png")
                texture = EditorTextureLoader::GetOrLoadTexture(file.path().string());
            else
            {
                std::string textureType = "file";
                if (file.path().extension() == ".glsl")
                    textureType = "shader";

                texture = EditorTextureLoader::GetTextureID(textureType);
            }

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
                std::string payloadType = "file";
                if (file.path().extension() == ".h" || file.path().extension() == ".cpp")
                    payloadType = "script";
                else if (file.path().extension() == ".glsl")
                    payloadType = "glsl";

                ImGui::SetDragDropPayload(payloadType.c_str(), file.path().string().c_str(), file.path().string().size() + 1);
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
            ImGui::NextColumn();
        }

        ImGui::Columns(1);
        ImGui::PopStyleVar();

        ImGui::Unindent(left_padding);
        ImGui::End();
    }
}

#endif