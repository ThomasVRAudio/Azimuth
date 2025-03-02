#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorPropertiesPanel.h>
#include <Azimuth/Project/FileGenerator.h>
#include <Azimuth/ECS/Components/ScriptContainerComponent.h>
#include <Azimuth/Editor/EditorManager.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/System/Files.h>
#include <Azimuth/Scripts/ScriptModuleLoader.h>
#include <Azimuth/Editor/EditorPlayState.h>

namespace Azimuth
{
    EditorFilepicker EditorPropertiesPanel::m_Filepicker;

    void EditorPropertiesPanel::DrawPanel()
    {
        float left_padding = 10.0f;
        ImGui::Begin("Properties");
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Indent(left_padding);

        if (EditorManager::m_Scene->HasComponent<TagComponent>(EditorManager::m_SelectedEntity))
        {
            TagComponent &component = EditorManager::m_Scene->GetComponent<TagComponent>(EditorManager::m_SelectedEntity);
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

        if (EditorManager::m_Scene->HasComponent<TransformComponent>(EditorManager::m_SelectedEntity))
        {
            ImGui::Text("Transform");
            TransformComponent &component = EditorManager::m_Scene->GetComponent<TransformComponent>(EditorManager::m_SelectedEntity);

            DrawVec3Box(component.Position, "Translate", {"X", "Y", "Z"});

            glm::vec3 rotInDeg = glm::degrees(component.Rotation);
            DrawVec3Box(rotInDeg, "Rotate", {"X", "Y", "Z"}, 0.1f);
            component.Rotation = glm::radians(rotInDeg);

            DrawVec3Box(component.Scale, "Scale", {"X", "Y", "Z"});

            ImGui::Separator();
        }

        if (EditorManager::m_Scene->HasComponent<LightComponent>(EditorManager::m_SelectedEntity))
        {
            ImGui::Text("Light");
            LightComponent &component = EditorManager::m_Scene->GetComponent<LightComponent>(EditorManager::m_SelectedEntity);

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
                            EditorManager::UpdateLights();
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
            ImGui::SliderFloat("Intensity", &component.Intensity, 0.0f, 20.0f);
            ImGui::PopItemWidth();

            ImGui::Separator();
        }

        if (EditorManager::m_Scene->HasComponent<MeshComponent>(EditorManager::m_SelectedEntity))
        {
            MeshComponent &component = EditorManager::m_Scene->GetComponent<MeshComponent>(EditorManager::m_SelectedEntity);
            const char *items[] = {"None", "Point", "Line", "Triangle", "Square", "Cube"};
            auto currentItem = component.GetMeshType();

            ImGui::Text("Mesh");
            if (ImGui::Button("Select Mesh"))
            {
                m_SelectedFilepicker = MESH_PICKER;
                m_Filepicker.SetOpenWindow(true);
            }

            if (m_Filepicker.IsOpen() && m_SelectedFilepicker == MESH_PICKER)
            {

                std::function<void()> handleGeometrySelectionFunction = []()
                {
                    ImGui::SeparatorText("Geometry:");

                    std::string geometryOptions[6] = {"None", "Point", "Line", "Triangle", "Square", "Cube"};

                    for (size_t i = 0; i < 6; ++i)
                    {
                        if (ImGui::Selectable(geometryOptions[i].c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
                        {
                            MeshComponent &component = EditorManager::m_Scene->GetComponent<MeshComponent>(EditorManager::m_SelectedEntity);
                            component.UpdateMeshGeometry(static_cast<GEOMETRY_TYPE>(i));
                            m_Filepicker.SetOpenWindow(false);
                        }
                    }
                };

                std::vector<std::string> extensions{".obj"};
                const std::string &file = m_Filepicker.SelectFile(Application::projectSettings->ProjectFolder.string().c_str(), "Select Mesh", extensions, false, handleGeometrySelectionFunction);
                if (file.length() > 0)
                {
                    MeshComponent &component = EditorManager::m_Scene->GetComponent<MeshComponent>(EditorManager::m_SelectedEntity);
                    std::shared_ptr<Model> model = std::make_shared<Model>(file.c_str());
                    component.UpdateMeshModel(model);
                }
            }

            ImGui::Separator();
        }

        if (EditorManager::m_Scene->HasComponent<MaterialComponent>(EditorManager::m_SelectedEntity))
        {
            ImGui::Text("Material");
            MaterialComponent &material = EditorManager::m_Scene->GetComponent<MaterialComponent>(EditorManager::m_SelectedEntity);
            ImGui::PushItemWidth(100.0f);

            std::string currentItem = "";

            std::string selectedShader = material.shader->GetPaths().second;

            size_t lastSlash = selectedShader.find_last_of("/\\");
            if (lastSlash != std::string::npos)
                selectedShader = selectedShader.substr(lastSlash + 1);

            size_t lastDot = selectedShader.find_last_of('.');
            if (lastDot != std::string::npos)
                selectedShader = selectedShader.substr(0, lastDot);

            ImGui::Text("Shader: ");
            ImGui::SameLine(70);
            if (ImGui::Button(selectedShader.c_str()))
                ImGui::OpenPopup("Shader File Explorer");

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("glsl"))
                {
                    const char *droppedFilePath = static_cast<const char *>(payload->Data);

                    MaterialComponent &material = EditorManager::m_Scene->GetComponent<MaterialComponent>(EditorManager::m_SelectedEntity);

                    std::shared_ptr<Shader> shader = std::make_shared<Shader>(droppedFilePath);
                    material.shader = shader;
                    material.SetUniforms();
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 10.0f));
            if (ImGui::BeginPopup("Shader File Explorer"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
                ImGui::Text(" Shader Select ");
                ShaderDirectoryCombo("assets/shaders/library", currentItem, material);
                ImGui::PopStyleVar();

                ImGui::EndPopup();
            }
            ImGui::PopStyleVar();

            float left_padding = 10.0f;
            unsigned int sampleSlot = 0;

            bool isTexturePickerOpened = false;

            for (auto &uniform : *material.GetUniforms())
            {
                ImGui::PushItemWidth(150.0f);

                if (uniform.Name.find("g_") != std::string::npos)
                    continue;

                std::string uniformName = uniform.Name;
                if (uniformName.substr(0, 2) == "u_")
                    uniformName = uniformName.substr(2);

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

                    ImGui::SliderFloat(uniformName.c_str(), &value, 0.0f, max);
                }
                break;

                case GL_INT:
                {
                    int &value = std::get<int>(uniform.Value);
                    ImGui::SliderInt(uniformName.c_str(), &value, -100, 100);
                }
                break;

                case GL_UNSIGNED_INT:
                {
                    int &value = std::get<int>(uniform.Value);
                    ImGui::SliderInt(uniformName.c_str(), &value, 0, 1000);
                }
                break;

                case GL_SAMPLER_2D:
                {
                    ImGui::Text(uniform.Name.c_str());
                    std::string name = uniform.Name;
                    if (ImGui::Button(("Select Texture##" + name).c_str()))
                    {
                        m_SelectedTextureName = uniform.Name;
                        m_SelectedTextureSlot = sampleSlot;
                        m_SelectedFilepicker = TEXTURE_PICKER;
                        m_Filepicker.SetOpenWindow(true);
                    }

                    if (ImGui::BeginDragDropTarget())
                    {
                        m_SelectedTextureName = uniform.Name;
                        m_SelectedTextureSlot = sampleSlot;

                        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("file"))
                        {
                            const char *droppedFilePath = static_cast<const char *>(payload->Data);

                            MaterialComponent &component = EditorManager::m_Scene->GetComponent<MaterialComponent>(EditorManager::m_SelectedEntity);
                            component.AddTexture(m_SelectedTextureName.c_str(), droppedFilePath, m_SelectedTextureSlot);
                        }
                        ImGui::EndDragDropTarget();
                    }

                    if (m_Filepicker.IsOpen() && m_SelectedFilepicker == TEXTURE_PICKER && !isTexturePickerOpened)
                    {
                        std::vector<std::string> extensions{".png", ".jpg"};
                        const std::string &file = m_Filepicker.SelectFile("assets/Assets/textures", "Select Texture", extensions, false);
                        if (file.length() > 0)
                        {
                            MaterialComponent &component = EditorManager::m_Scene->GetComponent<MaterialComponent>(EditorManager::m_SelectedEntity);
                            component.AddTexture(m_SelectedTextureName.c_str(), file.c_str(), m_SelectedTextureSlot);
                        }

                        isTexturePickerOpened = true;
                    }

                    sampleSlot++;
                }
                break;

                case GL_BOOL:
                {
                    bool &value = std::get<bool>(uniform.Value);
                    ImGui::Checkbox(uniformName.c_str(), &value);
                }
                break;

                case GL_FLOAT_VEC3:
                {

                    glm::vec3 &value = std::get<glm::vec3>(uniform.Value);
                    if (uniform.Name.find("u_Color") != std::string::npos)
                    {
                        ImGui::ColorPicker3(uniformName.c_str(), &value[0], ImGuiColorEditFlags_NoInputs);
                    }
                    else
                    {
                        ImGui::SliderFloat3(uniformName.c_str(), &value[0], -1.0f, 1.0f);
                    }
                }
                break;

                case GL_FLOAT_VEC4:
                {
                    glm::vec4 &value = std::get<glm::vec4>(uniform.Value);
                    ImGui::SliderFloat4(uniformName.c_str(), &value[0], -1.0f, 1.0f);
                }
                break;

                default:
                    break;
                }
                ImGui::PopItemWidth();
            }
            ImGui::Separator();
        }

        if (EditorManager::m_Scene->HasComponent<AudioComponent>(EditorManager::m_SelectedEntity))
        {
            ImGui::Text("AudioComponent");
            ImGui::Separator();
        }

        if (EditorManager::m_Scene->HasComponent<ScriptContainerComponent>(EditorManager::m_SelectedEntity))
        {
            ScriptContainerComponent &container = EditorManager::m_Scene->GetComponent<ScriptContainerComponent>(EditorManager::m_SelectedEntity);

            for (const auto &script : container.GetScriptPaths())
            {

                ImGui::Text("Script");

                char scriptBuffer[512];
                strncpy(scriptBuffer, script.string().c_str(), sizeof(scriptBuffer) - 1);
                scriptBuffer[sizeof(scriptBuffer) - 1] = '\0';

                ImGui::InputText(("##ScriptComponent" + script.string()).c_str(), scriptBuffer, IM_ARRAYSIZE(scriptBuffer), ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoUndoRedo);
                if (ImGui::BeginDragDropTarget())
                {
                    m_SelectedScriptPath = script;

                    if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("script"))
                    {
                        const char *droppedFilePath = static_cast<const char *>(payload->Data);

                        ScriptContainerComponent &component = EditorManager::m_Scene->GetComponent<ScriptContainerComponent>(EditorManager::m_SelectedEntity);
                        component.ReplaceScriptPathRelative(m_SelectedScriptPath, droppedFilePath);
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::Separator();
            }
        }

        if (m_IsAddingScript && EditorManager::m_SelectedEntity == selectedPropertiesEntity)
        {
            static char nameBuffer[256];
            ImGui::Text("Script Name: ");
            ImGui::SetKeyboardFocusHere();
            if (ImGui::InputText("##AddScript", nameBuffer, IM_ARRAYSIZE(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (nameBuffer[0] == '\0')
                {
                    print("Script name can't be empty");
                }
                else
                {
                    std::filesystem::path scriptPath = Application::projectSettings->ProjectFolder;

                    FileGenerator::GenerateScripts(scriptPath, nameBuffer);
                    ScriptContainerComponent &component = EditorManager::m_Scene->GetComponent<ScriptContainerComponent>(selectedPropertiesEntity);
                    component.AddScriptPath(std::string(nameBuffer) + ".h");
                    m_IsAddingScript = false;
                    nameBuffer[0] = '\0';
                    ScriptModuleLoader::LoadModule();
                }
            }
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

        ImGui::BeginDisabled(EditorPlayState::GetPlayState() != PlayState::STOPPED);
        if (ImGui::Button("Add Component", ImVec2(buttonWidth, buttonHeight)))
            ImGui::OpenPopup("Select Component");

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("script"))
            {
                const char *droppedFilePath = static_cast<const char *>(payload->Data);

                ScriptContainerComponent &component = EditorManager::m_Scene->GetComponent<ScriptContainerComponent>(EditorManager::m_SelectedEntity);
                component.AddScriptPath(std::filesystem::path(droppedFilePath).filename().stem().string() + ".h");
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::EndDisabled();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        if (ImGui::BeginPopup("Select Component"))
        {
            if (!EditorManager::m_Scene->HasComponent<TransformComponent>(EditorManager::m_SelectedEntity))
            {
                if (ImGui::Selectable("Transform"))
                {
                    TransformComponent component;
                    EditorManager::m_Scene->AddComponent<TransformComponent>(EditorManager::m_SelectedEntity, std::move(component));
                }
            }

            if (!EditorManager::m_Scene->HasComponent<MeshComponent>(EditorManager::m_SelectedEntity))
            {
                if (ImGui::Selectable("Mesh"))
                {
                    MeshComponent component;
                    component.UpdateMeshGeometry(GEOMETRY_CUBE);
                    EditorManager::m_Scene->AddComponent<MeshComponent>(EditorManager::m_SelectedEntity, std::move(component));

                    if (!EditorManager::m_Scene->HasComponent<MaterialComponent>(EditorManager::m_SelectedEntity))
                    {
                        MaterialComponent mat;
                        mat.CreateMaterial();
                        EditorManager::m_Scene->AddComponent<MaterialComponent>(EditorManager::m_SelectedEntity, std::move(mat));
                    }
                }
            }

            if (!EditorManager::m_Scene->HasComponent<LightComponent>(EditorManager::m_SelectedEntity))
            {
                if (ImGui::Selectable("Light"))
                {
                    LightComponent component;
                    EditorManager::m_Scene->AddComponent<LightComponent>(EditorManager::m_SelectedEntity, std::move(component));
                    EditorManager::UpdateLights();
                };
            }

            if (ImGui::Selectable("Script"))
            {
                m_IsAddingScript = true;
                selectedPropertiesEntity = EditorManager::m_SelectedEntity;
            }

            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }

    void EditorPropertiesPanel::ShaderDirectoryCombo(const std::string &path, std::string &currentItem, MaterialComponent &material)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 5.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 5.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 5.0f);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

        std::vector<std::pair<FileType, std::string>> filenames = Files::GetFilenamesFromDirectory(path);

        for (const auto &filename : filenames)
        {
            const std::string fullPath = path + "/" + filename.second;

            if (filename.first == FileType::Directory)
            {
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
                if (ImGui::TreeNodeEx(filename.second.c_str()))
                {
                    ShaderDirectoryCombo(fullPath, currentItem, material);
                    ImGui::TreePop();
                }
                ImGui::PopFont();
            }
            else
            {
                if (filename.second.find(".vert") != std::string::npos)
                    continue;

                bool isSelected = (currentItem == filename.second);
                std::string fileNoExtension = filename.second.substr(0, filename.second.find_last_of('.'));
                ImGui::Indent(14.0f);
                if (ImGui::Selectable(fileNoExtension.c_str(), isSelected))
                {
                    std::string vertPath;
                    bool lit;
                    if (Files::GetShaderInfoFromFile(path + "/" + filename.second, vertPath, lit))
                    {
                        std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertPath, path + "/" + filename.second, lit);
                        material.shader = shader;
                        material.SetUniforms();
                    }
                    else
                    {
                        print("Shader Info not found for: " << path);
                    }
                }
                ImGui::Unindent();
            }
        }

        ImGui::PopFont();
        ImGui::PopStyleVar(4);
    }
}

#endif