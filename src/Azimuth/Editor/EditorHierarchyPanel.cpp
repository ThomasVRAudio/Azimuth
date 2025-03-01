#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorHierarchyPanel.h>
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Editor/EditorManager.h>
#include <Azimuth/Editor/EditorPlayState.h>

namespace Azimuth
{
    void EditorHierarchyPanel::DrawPanel()
    {
        float left_padding = 10.0f;
        ImGui::Begin("Hierarchy");
        ImGui::Indent(left_padding);
        ImGui::Dummy(ImVec2(4.0f, 4.0f));

        static bool isRenaming = false;
        static Entity selectedEntityForRenaming;
        static char buffer[256];

        for (auto &entity : EditorManager::m_Scene->m_Entities)
        {
            std::string name = EditorManager::m_Scene->GetComponent<TagComponent>(entity).name;
            bool isSelected = (EditorManager::m_SelectedEntity == entity);
            if (ImGui::Selectable(name.c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick))
            {
                EditorManager::m_SelectedEntity = entity;
            }

            if (isSelected && ImGui::IsKeyPressed(ImGuiKey_F2))
            {
                isRenaming = true;
                selectedEntityForRenaming = entity;
                strncpy(buffer, name.c_str(), sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
            }

            if (isRenaming && selectedEntityForRenaming == entity)
            {
                ImGui::SetKeyboardFocusHere();
                if (ImGui::InputText("##Rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    EditorManager::m_Scene->GetComponent<TagComponent>(entity).name = buffer;
                    isRenaming = false;
                }
            }
        }

        float fullWidth = ImGui::GetContentRegionAvail().x;
        float windowHeight = ImGui::GetWindowSize().y;
        float buttonHeight = 30.0f;
        float margin = 20.0f;

        float buttonWidth = fullWidth - margin;
        float centerX = (fullWidth - buttonWidth) * 0.5f;

        ImGui::SetCursorPosX(centerX);

        ImGui::SetCursorPosY(windowHeight - buttonHeight - margin);

        ImGui::BeginDisabled(EditorPlayState::GetPlayState() != PlayState::STOPPED);
        if (ImGui::Button("Add Gameobject", ImVec2(buttonWidth, buttonHeight)))
        {
            Entity entity = EditorManager::m_Scene->CreateEntity("Gameobject");
            std::string &name = EditorManager::m_Scene->GetComponent<TagComponent>(entity).name;
            name += std::to_string(entity);
            EditorManager::m_SelectedEntity = entity;
        }
        ImGui::EndDisabled();

        ImGui::Unindent(left_padding);
        ImGui::End();
    };
}

#endif