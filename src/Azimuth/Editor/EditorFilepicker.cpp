#include <Azimuth/Editor/EditorFilepicker.h>
#ifdef AZIMUTH_EDITOR

namespace Azimuth
{

    void EditorFilepicker::DrawPanel(const char *path, std::string panelName, bool dockable)
    {
        ImGuiWindowFlags flags = 0;
        if (!dockable)
            flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;

        ImGui::Begin(panelName.c_str(), &m_IsOpen, flags);
        ImGui::SetWindowPos(ImVec2(Window::GetWidth() / 3.0f, Window::GetHeight() / 3.0f), ImGuiCond_Once);
        ImGui::SetWindowSize(ImVec2(300, 400), ImGuiCond_Once);

        float left_padding = 10.0f;
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Indent(left_padding);

        ImGui::SeparatorText("Geometry:");

        std::string geometryOptions[6] = {"None", "Point", "Line", "Triangle", "Square", "Cube"};

        for (size_t i = 0; i < 6; ++i)
        {
            if (ImGui::Selectable(geometryOptions[i].c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
            {
                m_IsOpen = false;
                MeshComponent &component = EditorUI::m_Scene->GetComponent<MeshComponent>(EditorUI::m_SelectedEntity);
                component.UpdateMeshGeometry(static_cast<GEOMETRY_TYPE>(i));
            }
        }

        std::vector<std::filesystem::path> filenames = Files::GetFilesWithExtension(path, ".obj");

        ImGui::SeparatorText("Assets:");

        for (const auto &file : filenames)
        {
            std::string filename = file.filename().stem().string();
            if (ImGui::Selectable(filename.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
            {
                m_IsOpen = false;
                MeshComponent &component = EditorUI::m_Scene->GetComponent<MeshComponent>(EditorUI::m_SelectedEntity);
                std::shared_ptr<Model> model = std::make_shared<Model>(file.string().c_str());
                component.UpdateMeshModel(model);
            }
        }

        ImGui::Unindent(left_padding);
        ImGui::End();
    }

}

#endif