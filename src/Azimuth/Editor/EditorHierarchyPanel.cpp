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
        for (auto &entity : EditorUI::m_Scene->m_Entities)
        {
            std::string name = EditorUI::m_Scene->GetComponent<TagComponent>(entity).name;
            bool isSelected = (EditorUI::m_SelectedEntity == entity);
            if (ImGui::Selectable(name.c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick))
            {
                EditorUI::m_SelectedEntity = entity;
            }
        }

        ImGui::Unindent(left_padding);
        ImGui::End();
    };
}

#endif