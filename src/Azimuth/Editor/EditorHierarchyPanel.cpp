#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorHierarchyPanel.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Editor/EditorUI.h>

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

        for (auto &entity : EditorUI::m_Scene->m_Entities)
        {
            std::string name = EditorUI::m_Scene->GetComponent<TagComponent>(entity).name;
            bool isSelected = (EditorUI::m_SelectedEntity == entity);
            if (ImGui::Selectable(name.c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick))
            {
                EditorUI::m_SelectedEntity = entity;
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
                    EditorUI::m_Scene->GetComponent<TagComponent>(entity).name = buffer;
                    isRenaming = false;
                }
            }
        }

        float fullWidth = ImGui::GetContentRegionAvail().x;
        float windowHeight = ImGui::GetWindowSize().y;
        float buttonHeight = 30.0f;
        float margin = 30.0f;

        ImGui::SetCursorPosY(windowHeight - buttonHeight - margin);

        if (ImGui::Button("Add Gameobject", ImVec2(fullWidth - margin, buttonHeight)))
        {
            Entity entity = EditorUI::m_Scene->CreateEntity("Gameobject");
            std::string &name = EditorUI::m_Scene->GetComponent<TagComponent>(entity).name;
            name += std::to_string(entity);
            EditorUI::m_SelectedEntity = entity;
        }

        ImGui::Unindent(left_padding);
        ImGui::End();
    };
}

#endif