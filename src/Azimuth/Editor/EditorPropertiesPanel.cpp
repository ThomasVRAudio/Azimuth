#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorPropertiesPanel.h>

namespace Azimuth
{
    void EditorPropertiesPanel::DrawPanel()
    {
        float left_padding = 10.0f;
        ImGui::Begin("Properties");
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Indent(left_padding);

        if (EditorUI::m_Scene->HasComponent<TagComponent>(EditorUI::m_SelectedEntity))
        {
            TagComponent &component = EditorUI::m_Scene->GetComponent<TagComponent>(EditorUI::m_SelectedEntity);
            static char name[256];
            if (strlen(name) == 0)
            {
                strncpy(name, component.name.c_str(), sizeof(name) - 1);
                name[sizeof(name) - 1] = '\0';
            }
            ImGui::Text("Name Tag");
            ImGui::SameLine();
            ImGui::PushItemWidth(200.0f);
            if (ImGui::InputText("##Name", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (strlen(name) > 0)
                    component.name = name;
            }
            ImGui::PopItemWidth();
            ImGui::Separator();
        }

        if (EditorUI::m_Scene->HasComponent<TransformComponent>(EditorUI::m_SelectedEntity))
        {
            ImGui::Text("Transform");
            TransformComponent &component = EditorUI::m_Scene->GetComponent<TransformComponent>(EditorUI::m_SelectedEntity);

            DrawVec3Box(component.Position, "Translate", {"X", "Y", "Z"});

            glm::vec3 rotInDeg = glm::degrees(component.Rotation);
            DrawVec3Box(rotInDeg, "Rotate", {"X", "Y", "Z"}, 0.1f);
            component.Rotation = glm::radians(rotInDeg);

            DrawVec3Box(component.Scale, "Scale", {"X", "Y", "Z"});

            ImGui::Separator();
        }

        if (EditorUI::m_Scene->HasComponent<LightComponent>(EditorUI::m_SelectedEntity))
        {
            ImGui::Text("Light");
            LightComponent &component = EditorUI::m_Scene->GetComponent<LightComponent>(EditorUI::m_SelectedEntity);

            const char *items[] = {"Point", "Directional", "Spot"};
            auto currentItem = component.Type;

            ImGui::Text("Type");
            ImGui::SameLine();
            ImGui::PushItemWidth(100.0f);
            if (ImGui::BeginCombo("##TypeCombo", items[currentItem]))
            {
                for (int i = 0; i < IM_ARRAYSIZE(items); i++)
                {
                    bool isSelected = (currentItem == i);
                    if (ImGui::Selectable(items[i], isSelected))
                    {
                        if (currentItem != i)
                        {
                            currentItem = static_cast<LightType>(i);
                            component.Type = currentItem;
                            EditorUI::UpdateLights();
                        }
                    }
                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::Checkbox("Active", &component.IsActive);
            ImGui::ColorPicker3("Light Color", &component.Color[0], ImGuiColorEditFlags_NoInputs);
            ImGui::SliderFloat("HDR Multiplier", &component.HDRMultiplier, 0.0f, 10.0f);
            ImGui::PopItemWidth();

            ImGui::Separator();
        }

        if (EditorUI::m_Scene->HasComponent<MeshComponent>(EditorUI::m_SelectedEntity))
        {
            MeshComponent &component = EditorUI::m_Scene->GetComponent<MeshComponent>(EditorUI::m_SelectedEntity);
            const char *items[] = {"None", "Point", "Line", "Triangle", "Square", "Cube"};
            auto currentItem = component.GetMeshType();

            ImGui::Text("Mesh");
            ImGui::Text("Shape");
            ImGui::SameLine();
            ImGui::PushItemWidth(100.0f);
            if (ImGui::BeginCombo("##MeshCombo", items[currentItem]))
            {
                for (int i = 0; i < IM_ARRAYSIZE(items); i++)
                {
                    bool isSelected = (currentItem == i);
                    if (ImGui::Selectable(items[i], isSelected))
                    {
                        if (currentItem != i)
                        {
                            currentItem = static_cast<GEOMETRY_TYPE>(i);
                            component.UpdateMeshGeometry(static_cast<GEOMETRY_TYPE>(i));
                        }
                    }
                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();
            ImGui::Separator();
        }

        if (EditorUI::m_Scene->HasComponent<MaterialComponent>(EditorUI::m_SelectedEntity))
        {
            ImGui::Text("Material");
            MaterialComponent &material = EditorUI::m_Scene->GetComponent<MaterialComponent>(EditorUI::m_SelectedEntity);
            ImGui::PushItemWidth(100.0f);

            for (auto &uniform : *material.GetUniforms())
            {
                ImGui::PushItemWidth(150.0f);

                if (uniform.Name.find("g_") != std::string::npos)
                    continue;

                switch (uniform.Type)
                {
                case GL_FLOAT:
                {
                    float &value = std::get<float>(uniform.Value);
                    float max = 1.0f;

                    if (uniform.Name.find("shininess")) // TO DO
                        max = 100.0f;

                    if (uniform.Name.find("HDR"))
                        max = 10.0f;

                    ImGui::SliderFloat(uniform.Name.c_str(), &value, 0.0f, max);
                }
                break;

                case GL_INT:
                {
                    int &value = std::get<int>(uniform.Value);
                    ImGui::SliderInt(uniform.Name.c_str(), &value, -100, 100);
                }
                break;

                case GL_UNSIGNED_INT:
                {
                    int &value = std::get<int>(uniform.Value);
                    ImGui::SliderInt(uniform.Name.c_str(), &value, 0, 1000);
                }
                break;

                case GL_BOOL:
                {
                    bool &value = std::get<bool>(uniform.Value);
                    ImGui::Checkbox(uniform.Name.c_str(), &value);
                }
                break;

                case GL_FLOAT_VEC3:
                {

                    glm::vec3 &value = std::get<glm::vec3>(uniform.Value);
                    if (uniform.Name.find("u_Color") != std::string::npos)
                    {
                        ImGui::ColorPicker3(uniform.Name.c_str(), &value[0], ImGuiColorEditFlags_NoInputs);
                    }
                    else
                    {
                        ImGui::SliderFloat3(uniform.Name.c_str(), &value[0], -1.0f, 1.0f);
                    }
                }
                break;

                case GL_FLOAT_VEC4:
                {
                    glm::vec4 &value = std::get<glm::vec4>(uniform.Value);
                    ImGui::SliderFloat4(uniform.Name.c_str(), &value[0], -1.0f, 1.0f);
                }
                break;

                default:
                    break;
                }
                ImGui::PopItemWidth();
            }
            ImGui::Separator();
        }

        if (EditorUI::m_Scene->HasComponent<AudioComponent>(EditorUI::m_SelectedEntity))
        {
            ImGui::Text("AudioComponent");
            ImGui::Separator();
        }

        AddComponent();

        ImGui::Unindent(left_padding);
        ImGui::End();
    }

    void EditorPropertiesPanel::DrawVec3Box(glm::vec3 &vec3, std::string title, const std::array<std::string, 3> &labels, float speed)
    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s: ", title.c_str());
        ImGui::SameLine(100);

        ImGui::PushItemWidth(60.0f);
        ImGui::Text(labels[0].c_str());
        ImGui::SameLine();
        ImGui::DragFloat(("##" + title + labels[0]).c_str(), &vec3.x, speed);

        ImGui::SameLine();
        ImGui::Text("%s", labels[1].c_str());
        ImGui::SameLine();
        ImGui::DragFloat(("##" + title + labels[1]).c_str(), &vec3.y, speed);

        ImGui::SameLine();
        ImGui::Text("%s", labels[2].c_str());
        ImGui::SameLine();
        ImGui::DragFloat(("##" + title + labels[2]).c_str(), &vec3.z, speed);
        ImGui::PopItemWidth();
    }

    void EditorPropertiesPanel::AddComponent()
    {

        float fullWidth = ImGui::GetContentRegionAvail().x;
        float buttonHeight = 30.0f;
        float margin = 20.0f;
        float buttonWidth = fullWidth - margin;
        float centerX = (fullWidth - buttonWidth) * 0.5f;

        ImGui::SetCursorPosX(centerX);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + margin * 0.5f);

        if (ImGui::Button("Add Component", ImVec2(buttonWidth, buttonHeight)))
            ImGui::OpenPopup("Select Component");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        if (ImGui::BeginPopup("Select Component"))
        {
            if (!EditorUI::m_Scene->HasComponent<TransformComponent>(EditorUI::m_SelectedEntity))
            {
                if (ImGui::Selectable("Transform"))
                {
                    TransformComponent component;
                    EditorUI::m_Scene->AddComponent<TransformComponent>(EditorUI::m_SelectedEntity, std::move(component));
                }
            }

            if (!EditorUI::m_Scene->HasComponent<MeshComponent>(EditorUI::m_SelectedEntity))
            {
                if (ImGui::Selectable("Mesh"))
                {
                    MeshComponent component;
                    component.UpdateMeshGeometry(GEOMETRY_CUBE);
                    EditorUI::m_Scene->AddComponent<MeshComponent>(EditorUI::m_SelectedEntity, std::move(component));

                    if (!EditorUI::m_Scene->HasComponent<MaterialComponent>(EditorUI::m_SelectedEntity))
                    {
                        MaterialComponent mat;
                        mat.CreateMaterial();
                        EditorUI::m_Scene->AddComponent<MaterialComponent>(EditorUI::m_SelectedEntity, std::move(mat));
                    }
                }
            }

            if (!EditorUI::m_Scene->HasComponent<LightComponent>(EditorUI::m_SelectedEntity))
            {
                if (ImGui::Selectable("Light"))
                {
                    LightComponent component;
                    EditorUI::m_Scene->AddComponent<LightComponent>(EditorUI::m_SelectedEntity, std::move(component));
                    EditorUI::UpdateLights();
                };
            }

            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }
}

#endif