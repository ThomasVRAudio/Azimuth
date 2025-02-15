#include <Azimuth/Project/Serializer.h>

namespace Azimuth
{

    bool Serializer::OpenFileDialog(std::string &outFilePath, FileDialogType dialogType)
    {
        OPENFILENAME ofn;
        char szFile[1024] = {0};

        ofn.hwndOwner = glfwGetWin32Window(Window::GetMainWindow());

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "Scene Files\0*.scene\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        std::filesystem::path projectPath = std::filesystem::current_path();

        if (projectPath.filename() != "Scenes")
            projectPath /= "Scenes";

        if (!std::filesystem::exists(projectPath))
            std::filesystem::create_directories(projectPath);

        std::string initialDirString = std::filesystem::absolute(projectPath).string();
        ofn.lpstrInitialDir = initialDirString.c_str();

        ofn.lpstrTitle = "Open Scene File";

        ofn.Flags = 0;
        ofn.Flags |= OFN_NOCHANGEDIR;

        if (dialogType == SAVE)
            ofn.Flags |= OFN_OVERWRITEPROMPT;
        else
            ofn.Flags |= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        BOOL result = (dialogType == OPEN) ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn);

        if (result == TRUE)
        {
            outFilePath = szFile;
            return true;
        }
        else
        {
            DWORD error = CommDlgExtendedError();
            std::cerr << "Dialog failed with error code: " << error << std::endl;
            return false;
        }

        return false;
    }

    void Serializer::OpenScene(Scene *scene, std::string path)
    {
        std::string filePath;

        if (path.length())
            filePath = path;

        if (path.length() || OpenFileDialog(filePath, OPEN))
        {
            for (auto &e : scene->m_Entities)
            {
                scene->DestroyEntity(e);
            }
            scene->m_Entities.clear();

            std::ifstream fin(filePath);
            if (!fin.is_open())
            {
                std::cerr << "Failed to open file: " << filePath << std::endl;
                return;
            }

            YAML::Node root = YAML::Load(fin);

            YAML::Node sceneSection = root["Scene"];

            scene->Settings->Exposure = sceneSection["Exposure"].as<float>();
            scene->Settings->HDRCubemapIntensity = sceneSection["HDRCubemapIntensity"].as<float>();
            scene->Settings->VSync = sceneSection["VSync"].as<bool>();
            scene->Settings->BloomBlend = sceneSection["BloomBlend"].as<float>();
            scene->Settings->BloomThreshold = sceneSection["BloomThreshold"].as<float>();

            for (const auto &entityNode : root["Entities"])
            {
                YAML::Node entitySection = entityNode.second;

                if (!entitySection["Tag"] || !entitySection["ID"])
                {
                    print("Couldn't load entity; Missing Tag or ID");
                    return;
                }
                Entity entity = scene->CreateEntity(entitySection["Tag"].as<std::string>());

                if (entitySection["Transform"])
                {
                    TransformComponent &component = scene->GetComponent<TransformComponent>(entity);
                    const YAML::Node &transformNode = entitySection["Transform"];

                    component.Position = glm::vec3(
                        transformNode["Position"][0].as<float>(),
                        transformNode["Position"][1].as<float>(),
                        transformNode["Position"][2].as<float>());

                    component.Rotation = glm::vec3(
                        transformNode["Rotation"][0].as<float>(),
                        transformNode["Rotation"][1].as<float>(),
                        transformNode["Rotation"][2].as<float>());

                    component.Scale = glm::vec3(
                        transformNode["Scale"][0].as<float>(),
                        transformNode["Scale"][1].as<float>(),
                        transformNode["Scale"][2].as<float>());
                }

                if (entitySection["Mesh"])
                {
                    MeshComponent component;
                    GEOMETRY_TYPE type = static_cast<GEOMETRY_TYPE>(entitySection["Mesh"]["Type"].as<int>());
                    if (entitySection["Mesh"]["Model"].IsDefined())
                    {
                        std::string path = entitySection["Mesh"]["Model"].as<std::string>();
                        std::shared_ptr<Model> model = std::make_shared<Model>(path.c_str());
                        component.CreateMesh(model);
                    }
                    else
                    {
                        component.CreateMesh(type);
                    }
                    scene->AddComponent<MeshComponent>(entity, std::move(component));
                }

                if (entitySection["Material"])
                {
                    MaterialComponent component;
                    const YAML::Node &mat = entitySection["Material"];

                    std::string vertexPath = mat["VertexPath"].as<std::string>();
                    std::string fragmentPath = mat["FragmentPath"].as<std::string>();
                    bool receivesLight = mat["IsLit"].as<bool>();
                    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexPath, fragmentPath, receivesLight);

                    std::shared_ptr<std::vector<Uniform>> uniforms = std::make_shared<std::vector<Uniform>>();
                    for (const auto &uniform : mat["Uniforms"])
                    {
                        Uniform u;
                        u.Name = uniform["Uniform"]["Name"].as<std::string>();
                        u.Type = static_cast<GLenum>(uniform["Uniform"]["Type"].as<int>());
                        switch (u.Type)
                        {
                        case static_cast<int>(GL_FLOAT):
                        {
                            u.Value = uniform["Uniform"]["Value"].as<float>();
                            break;
                        }
                        case static_cast<int>(GL_INT):
                        {
                            u.Value = uniform["Uniform"]["Value"].as<int>();
                            break;
                        }
                        case static_cast<int>(GL_BOOL):
                        {
                            u.Value = uniform["Uniform"]["Value"].as<bool>();
                            break;
                        }
                        case static_cast<int>(GL_FLOAT_VEC3):
                        {
                            u.Value = glm::vec3(
                                uniform["Uniform"]["Value"][0].as<float>(),
                                uniform["Uniform"]["Value"][1].as<float>(),
                                uniform["Uniform"]["Value"][2].as<float>());
                            break;
                        }
                        case static_cast<int>(GL_FLOAT_VEC4):
                        {
                            u.Value = glm::vec4(
                                uniform["Uniform"]["Value"][0].as<float>(),
                                uniform["Uniform"]["Value"][1].as<float>(),
                                uniform["Uniform"]["Value"][2].as<float>(),
                                uniform["Uniform"]["Value"][3].as<float>());
                            break;
                        }
                        }
                        uniforms->emplace_back(u);
                    }

                    component.CreateMaterial(shader, uniforms);
                    scene->AddComponent<MaterialComponent>(entity, std::move(component));
                }

                if (entitySection["Light"])
                {
                    LightComponent component;
                    const YAML::Node &lightNode = entitySection["Light"];

                    component.Color = glm::vec3(
                        lightNode["Color"][0].as<float>(),
                        lightNode["Color"][1].as<float>(),
                        lightNode["Color"][2].as<float>());

                    component.Type = static_cast<LightType>(lightNode["Type"].as<int>());
                    component.IsActive = lightNode["IsActive"].as<bool>();
                    component.Intensity = lightNode["HDRMultiplier"].as<float>();

                    scene->AddComponent<LightComponent>(entity, std::move(component));
                }
            }

            EditorManager::UpdateLights();
        }
    }

    void Serializer::SaveScene(Scene *scene)
    {
        std::string filePath;
        if (OpenFileDialog(filePath, SAVE))
        {
            if (filePath.find_last_of(".") == std::string::npos)
                filePath += ".scene";

            ECSManager *ECS = scene->ECS;
            YAML::Node root;

            // Save Scene Settings
            YAML::Node sceneSection;
            sceneSection["Exposure"] = scene->Settings->Exposure;
            sceneSection["HDRCubemapIntensity"] = scene->Settings->HDRCubemapIntensity;
            sceneSection["VSync"] = scene->Settings->VSync;
            sceneSection["BloomThreshold"] = scene->Settings->BloomThreshold;
            sceneSection["BloomBlend"] = scene->Settings->BloomBlend;
            root["Scene"] = sceneSection;

            // Save Entities
            YAML::Node entitiesSection;
            for (auto &entity : scene->m_Entities)
            {
                YAML::Node entitySection;
                entitySection["ID"] = entity;

                if (ECS->HasComponent<TagComponent>(entity))
                {
                    auto component = ECS->GetComponent<TagComponent>(entity);
                    entitySection["Tag"] = component.name;
                };

                if (ECS->HasComponent<TransformComponent>(entity))
                {
                    auto component = ECS->GetComponent<TransformComponent>(entity);
                    YAML::Node node;
                    node["Position"] = YAML::Node(YAML::NodeType::Sequence);
                    node["Position"].push_back(component.Position.x);
                    node["Position"].push_back(component.Position.y);
                    node["Position"].push_back(component.Position.z);

                    node["Rotation"] = YAML::Node(YAML::NodeType::Sequence);
                    node["Rotation"].push_back(component.Rotation.x);
                    node["Rotation"].push_back(component.Rotation.y);
                    node["Rotation"].push_back(component.Rotation.z);

                    node["Scale"] = YAML::Node(YAML::NodeType::Sequence);
                    node["Scale"].push_back(component.Scale.x);
                    node["Scale"].push_back(component.Scale.y);
                    node["Scale"].push_back(component.Scale.z);

                    entitySection["Transform"] = node;
                };

                if (ECS->HasComponent<MeshComponent>(entity))
                {
                    auto component = ECS->GetComponent<MeshComponent>(entity);
                    YAML::Node node;

                    node["Type"] = static_cast<int>(component.GetMeshType());
                    if (component.m_Model != nullptr)
                        node["Model"] = static_cast<std::string>(component.m_Model->GetModelDirectory());

                    entitySection["Mesh"] = node;
                }

                if (ECS->HasComponent<AudioComponent>(entity))
                {
                    auto component = ECS->GetComponent<AudioComponent>(entity);
                    YAML::Node node;

                    entitySection["Audio"] = node;
                }

                if (ECS->HasComponent<MaterialComponent>(entity))
                {
                    auto component = ECS->GetComponent<MaterialComponent>(entity);
                    YAML::Node node;
                    std::pair<std::string, std::string> paths = component.shader->GetPaths();

                    node["VertexPath"] = paths.first;
                    node["FragmentPath"] = paths.second;
                    node["IsLit"] = component.shader->IsLit();

                    YAML::Node uniformsNode;
                    for (auto &uniform : *component.m_Uniforms)
                    {
                        if (uniform.Name.find("g_") != std::string::npos)
                            continue;

                        YAML::Node uniformNode;
                        uniformNode["Type"] = static_cast<int>(uniform.Type);
                        uniformNode["Name"] = uniform.Name;
                        switch (uniform.Type)
                        {
                        case GL_FLOAT:
                        {
                            uniformNode["Value"] = std::get<float>(uniform.Value);
                            break;
                        }
                        case GL_INT:
                        {
                            uniformNode["Value"] = std::get<int>(uniform.Value);
                            break;
                        }
                        case GL_BOOL:
                        {
                            uniformNode["Value"] = std::get<bool>(uniform.Value);
                            break;
                        }
                        case GL_FLOAT_VEC3:
                        {
                            auto &v = std::get<glm::vec3>(uniform.Value);
                            uniformNode["Value"] = YAML::Node(YAML::NodeType::Sequence);
                            uniformNode["Value"].push_back(v.x);
                            uniformNode["Value"].push_back(v.y);
                            uniformNode["Value"].push_back(v.z);
                            break;
                        }
                        case GL_FLOAT_VEC4:
                        {
                            auto &v = std::get<glm::vec4>(uniform.Value);
                            uniformNode["Value"] = YAML::Node(YAML::NodeType::Sequence);
                            uniformNode["Value"].push_back(v.x);
                            uniformNode["Value"].push_back(v.y);
                            uniformNode["Value"].push_back(v.z);
                            uniformNode["Value"].push_back(v.w);
                            break;
                        }
                        }
                        YAML::Node uniformWrapper;
                        uniformWrapper["Uniform"] = uniformNode;
                        uniformsNode.push_back(uniformWrapper);
                    };

                    node["Uniforms"] = uniformsNode;

                    entitySection["Material"] = node;
                }

                if (ECS->HasComponent<LightComponent>(entity))
                {
                    auto component = ECS->GetComponent<LightComponent>(entity);
                    YAML::Node node;

                    node["Color"] = YAML::Node(YAML::NodeType::Sequence);
                    node["Color"].push_back(component.Color.x);
                    node["Color"].push_back(component.Color.y);
                    node["Color"].push_back(component.Color.z);

                    node["Type"] = static_cast<int>(component.Type);
                    node["IsActive"] = static_cast<bool>(component.IsActive);
                    node["HDRMultiplier"] = static_cast<float>(component.Intensity);

                    entitySection["Light"] = node;
                }

                entitiesSection["Entity " + std::to_string(entity)] = entitySection;
            }
            root["Entities"] = entitiesSection;
            std::ofstream fout(filePath);
            if (fout.is_open())
            {
                fout << root;
                fout.close();
                print("Scene saved to: " << filePath);
            }
            else
            {
                print("Failed to open file for saving: " << filePath);
            }
        }
    }
}