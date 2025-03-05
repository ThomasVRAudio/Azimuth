#include <Azimuth/Project/FileGenerator.h>
#include <Azimuth/Project/Serializer.h>

namespace Azimuth
{
    void FileGenerator::GenerateScripts(std::filesystem::path directory, const std::string &name)
    {
        if (std::filesystem::exists(directory / (name + ".cpp")) || std::filesystem::exists(directory / (name + ".h")))
        {
            std::cerr << "Error: " << name << ".cpp or " << name << ".h already exists." << std::endl;
            return;
        }

        std::string cppTemplate =
            "#include \"{name}.h\"\n"
            "#include <Azimuth.h>\n\n"
            "namespace Azimuth\n"
            "{\n"
            "    void {name}::OnStart() {\n"
            "        // Add your start logic here\n"
            "    }\n\n"
            "    void {name}::OnUpdate() {\n"
            "        // Add your update logic here\n"
            "    }\n"
            "}\n";

        std::string headerTemplate =
            "#pragma once\n"
            "#include <Azimuth.h>\n\n"
            "namespace Azimuth\n"
            "{\n"
            "   class {name} : public MonoScript\n"
            "   {\n"
            "   public:\n"
            "       {name}() : MonoScript(__FILE__) {}\n"
            "\n"
            "       std::shared_ptr<MonoScript> Clone() const override\n"
            "       {\n"
            "           return std::make_shared<{name}>(*this);\n"
            "       }\n"
            "\n"
            "       void OnStart() override;\n"
            "       void OnUpdate() override;\n"
            "   };\n"
            "}\n";

        size_t pos;
        while ((pos = cppTemplate.find("{name}")) != std::string::npos)
        {
            cppTemplate.replace(pos, 6, name);
        }

        while ((pos = headerTemplate.find("{name}")) != std::string::npos)
        {
            headerTemplate.replace(pos, 6, name);
        }

        std::ofstream cppFile(directory / (name + ".cpp"));
        if (cppFile.is_open())
        {
            cppFile << cppTemplate;
            cppFile.close();
        }
        else
        {
            print("Failed to create .cpp file");
            return;
        }

        std::ofstream headerFile(directory / (name + ".h"));
        if (headerFile.is_open())
        {
            headerFile << headerTemplate;
            headerFile.close();
        }
        else
        {
            print("Failed to create .h file");
            return;
        }

        UpdateDLLExportFile(directory, name, Application::projectSettings->ProjectFolder);
    }

    void FileGenerator::UpdateDLLExportFile(const std::filesystem::path &filepath, const std::string &name, const std::filesystem::path &projectFolder)
    {
        std::filesystem::path targetPath = projectFolder / "azimuth" / "dllexport.h";
        if (!std::filesystem::exists(targetPath))
        {
            print("Project Files should be created first");
            return;
        }

        std::ifstream inputFile(targetPath);
        if (!inputFile.is_open())
        {
            std::cerr << "Failed to open dllexport.h for reading." << std::endl;
            return;
        }

        std::string line;
        std::string fileContent;
        bool registryFound = false;
        unsigned int lineNumber = 0;

        while (std::getline(inputFile, line))
        {
            lineNumber++;

            fileContent += line + "\n";

            if (lineNumber == 2)
            {
                std::filesystem::path relative = std::filesystem::relative(filepath, projectFolder);

                fileContent += "#include <" + relative.string() + "/" + name + ".h>\n";
            }

            if (!registryFound && line.find("//[Registry]") != std::string::npos)
            {
                std::string registry = "m_Scripts.emplace_back(std::make_shared<" + name + ">());\n";
                fileContent += registry;
                registryFound = true;
            }
        }

        inputFile.close();

        if (!registryFound)
        {
            std::cerr << "Error: //[Registry] not found in the file." << std::endl;
            return;
        }

        std::ofstream outputFile(targetPath);
        if (!outputFile.is_open())
        {
            std::cerr << "Failed to open dllexport.h for writing." << std::endl;
            return;
        }

        outputFile << fileContent;
        outputFile.close();
    }

    void FileGenerator::GenerateProjectFiles(const std::filesystem::path &filePath, Scene *scene)
    {
        GenerateProjectSettingsFile(filePath);
        Serializer::OpenProject((filePath.string() + ".azimuth"), nullptr);

        GenerateSceneFile(filePath.parent_path());
        GenerateDLLExportFiles(filePath.parent_path());
        GenerateCMakeFile(filePath.parent_path());

        Serializer::OpenScene(scene, Application::projectSettings->MainScenePath.string());
    }

    void FileGenerator::GenerateDLLExportFiles(const std::filesystem::path &folderPath)
    {
        std::filesystem::path targetDirectory = folderPath / "azimuth";
        if (std::filesystem::exists(targetDirectory))
        {
            print("DLL Export Files already created");
            return;
        }
        else
        {
            std::filesystem::create_directory(targetDirectory);
        }

        std::string headerFile =
            "#pragma once\n"
            "#include <Azimuth.h>\n"
            "\n"
            "#ifdef BUILD_SCRIPTS_DLL\n"
            "#define SCRIPTS_API __declspec(dllexport)\n"
            "#else\n"
            "#define SCRIPTS_API __declspec(dllimport)\n"
            "#endif\n"
            "\n"
            "namespace Azimuth\n"
            "{\n"
            "    class DLLExport : public ScriptModule\n"
            "    {\n"
            "    public:\n"
            "        SCRIPTS_API void Init() override\n"
            "        {\n"
            "           //[Registry]\n"
            "        }\n"
            "    };\n"
            "\n"
            "    extern \"C\" SCRIPTS_API DLLExport *GetModule()\n"
            "    {\n"
            "        static DLLExport exportInstance;\n"
            "        return &exportInstance;\n"
            "    }\n"
            "}\n";

        std::ofstream headerFileStream(targetDirectory / "dllexport.h");
        if (headerFileStream.is_open())
        {
            headerFileStream << headerFile;
            headerFileStream.close();
        }
        else
        {
            print("Failed to create dllexport.h file");
            return;
        }

        std::string cppFile =
            "#include \"dllexport.h\"";

        std::ofstream cppFileStream(targetDirectory / "dllexport.cpp");
        if (cppFileStream.is_open())
        {
            cppFileStream << cppFile;
            cppFileStream.close();
        }
        else
        {
            print("Failed to create dllexport.cpp file");
            return;
        }
    }

    void FileGenerator::GenerateCMakeFile(const std::filesystem::path &folderPath)
    {

        std::filesystem::path targetPath = folderPath / "CMakeLists.txt";
        if (std::filesystem::exists(targetPath))
        {
            print("CMake file already created");
            return;
        }

        std::string cmakeFile =
            "cmake_minimum_required(VERSION 3.16)\n"
            "project(EntityScripts)\n"
            "\n"
            "set(AZIMUTH_ENGINE_PATH \"{engine_path}\")\n"
            "\n"
            "include_directories(\n"
            "    ${AZIMUTH_ENGINE_PATH}/include\n"
            "    ${AZIMUTH_ENGINE_PATH}/include/Azimuth\n"
            "    ${CMAKE_SOURCE_DIR}\n"
            ")\n"
            "\n"
            "file(GLOB_RECURSE GAME_SCRIPTS \"${CMAKE_SOURCE_DIR}/*.cpp\")\n"
            "\n"
            "add_library(EntityScripts SHARED ${GAME_SCRIPTS} ${ENGINE_SOURCES})\n"
            "\n"
            "set_target_properties(EntityScripts PROPERTIES\n"
            "   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/azimuth\n"

            ")\n"
            "\n"
            "target_include_directories(EntityScripts PUBLIC ${CMAKE_SOURCE_DIR})\n"
            "\n"
            "target_compile_definitions(EntityScripts PRIVATE BUILD_SCRIPTS_DLL)\n";

        size_t pos;
        while ((pos = cmakeFile.find("{engine_path}")) != std::string::npos)
        {
            cmakeFile.replace(pos, 13, (std::filesystem::current_path().parent_path()).generic_string());
        }

        std::ofstream cmakeFileStream(targetPath);
        if (cmakeFileStream.is_open())
        {
            cmakeFileStream << cmakeFile;
            cmakeFileStream.close();
        }
        else
        {
            print("Failed to create CMakeLists.txt file");
            return;
        }
    }

    void FileGenerator::GenerateProjectSettingsFile(const std::filesystem::path &filePath)
    {
        std::filesystem::path targetDirectory = filePath.parent_path();
        print("project settings file path: " << filePath);

        std::string projDir = "ProjectDir: " + targetDirectory.generic_string();
        std::string sceneDir = "MainSceneDir: " + (targetDirectory / "scenes").generic_string() + "/default_scene.scene";

        std::string file = projDir + "\n" + sceneDir;

        std::ofstream fileStream(targetDirectory / (filePath.filename().string() + ".azimuth"));
        if (fileStream.is_open())
        {
            fileStream << file;
            fileStream.close();
        }
        else
        {
            print("Failed to create: " + filePath.filename().string() + ".azimuth");
            return;
        }
    }

    void FileGenerator::GenerateSceneFile(const std::filesystem::path &projectFolder)
    {
        std::filesystem::path targetDirectory = projectFolder / "scenes";
        if (!std::filesystem::exists(targetDirectory))
            std::filesystem::create_directory(targetDirectory);

        std::string file =
            "Scene:\n"
            "  Exposure: 1.0\n"
            "  HDRCubemapIntensity: 1.0\n"
            "  VSync: true\n"
            "  BloomThreshold: 1.0\n"
            "  BloomBlend: 0.5\n";

        std::ofstream fileStream(targetDirectory / "default_scene.scene");
        if (fileStream.is_open())
        {
            fileStream << file;
            fileStream.close();
        }
        else
        {
            print("Failed to create default scene file");
            return;
        }
    }

    void FileGenerator::GenerateGLSLFile(const std::filesystem::path &directory, const std::string &name)
    {

        if (std::filesystem::exists(directory / (name + ".glsl")))
        {
            std::cerr << "Error: " << name << ".glsl already exists." << std::endl;
            return;
        }

        std::string file =
            "===== VERTEX SHADER =====\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aNormal;\n"
            "layout (location = 2) in vec2 aTexCoords;\n"
            "\n"
            "out vec2 TexCoords;\n"
            "out vec3 FragPos;\n"
            "out vec3 Normal;\n"
            "\n"
            "#define AZIMUTH_MVP_UNIFORMS\n"
            "\n"
            "void main()\n"
            "{\n"
            "    TexCoords = aTexCoords;\n"
            "    FragPos = AZIMUTH_FRAG;\n"
            "    Normal = AZIMUTH_NORMAL;\n"
            "    gl_Position = AZIMUTH_POSITION;\n"
            "}\n"
            "===== FRAGMENT SHADER =====\n"
            "out vec4 FragColor;\n"
            "\n"
            "uniform vec3 u_Color;\n"
            "uniform float u_HDR;\n"
            "\n"
            "void main() {\n"
            "    FragColor = vec4(u_Color * u_HDR, 1.0);\n"
            "}\n";

        std::ofstream fileStream(directory / (name + ".glsl"));
        if (fileStream.is_open())
        {
            fileStream << file;
            fileStream.close();
        }
        else
        {
            print("Failed to create shader file");
            return;
        }
    }

    void FileGenerator::GenerateGLSLFileSimpleLit(const std::filesystem::path &directory, const std::string &name)
    {
        if (std::filesystem::exists(directory / (name + ".glsl")))
        {
            std::cerr << "Error: " << name << ".glsl already exists." << std::endl;
            return;
        }

        std::string file =
            "===== VERTEX SHADER =====\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aNormal;\n"
            "layout (location = 2) in vec2 aTexCoords;\n"
            "\n"
            "out vec2 TexCoords;\n"
            "out vec3 FragPos;\n"
            "out vec3 Normal;\n"
            "\n"
            "#define AZIMUTH_MVP_UNIFORMS\n"
            "\n"
            "void main()\n"
            "{\n"
            "    TexCoords = aTexCoords;\n"
            "    FragPos = AZIMUTH_FRAG;\n"
            "    Normal = AZIMUTH_NORMAL;\n"
            "    gl_Position = AZIMUTH_POSITION;\n"
            "}\n"
            "===== FRAGMENT SHADER =====\n"
            "out vec4 FragColor;\n"
            "\n"
            "in vec3 FragPos;\n"
            "in vec3 Normal;\n"
            "\n"
            "uniform vec3 u_DiffuseColor;\n"
            "uniform vec3 u_SpecularColor;\n"
            "uniform vec3 u_AmbientColor;\n"
            "\n"
            "uniform float u_AmbientStrength;\n"
            "uniform float u_DiffuseStrength;\n"
            "uniform float u_SpecularStrength;\n"
            "\n"
            "uniform float u_SpecularAmount;\n"
            "uniform float u_HDR;\n"
            "\n"
            "#define AZIMUTH_LIT_PROPERTIES\n"
            "\n"
            "vec3 CalcDirLight(vec3 viewDir) {\n"
            "    vec3 lightDir = normalize(-AZIMUTH_DIR_LIGHT.direction);\n"
            "    float diff = max(dot(Normal, lightDir), 0.0);\n"
            "\n"
            "    vec3 reflectDir = reflect(-lightDir, Normal);\n"
            "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_SpecularAmount);\n"
            "\n"
            "    vec3 ambient = AZIMUTH_DIR_LIGHT.ambient * u_AmbientColor * u_AmbientStrength;\n"
            "    vec3 diffuse = AZIMUTH_DIR_LIGHT.diffuse * diff * u_DiffuseColor * u_DiffuseStrength;\n"
            "    vec3 specular = AZIMUTH_DIR_LIGHT.diffuse * spec * u_SpecularColor * u_SpecularStrength;\n"
            "\n"
            "    return (ambient + diffuse + specular);\n"
            "}\n"
            "\n"
            "vec3 CalcPointLight(AZIMUTH_POINT_LIGHT light, vec3 fragPos, vec3 viewDir) {\n"
            "    vec3 lightDir = normalize(light.position - fragPos);\n"
            "    float diff = max(dot(Normal, lightDir), 0.0);\n"
            "\n"
            "    vec3 reflectDir = reflect(-lightDir, Normal);\n"
            "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_SpecularAmount);\n"
            "\n"
            "    vec3 ambient = light.ambient * u_AmbientColor * u_AmbientStrength;\n"
            "    vec3 diffuse = light.diffuse * diff * u_DiffuseColor * u_DiffuseStrength;\n"
            "    vec3 specular = light.diffuse * spec * u_SpecularColor * u_SpecularStrength;\n"
            "\n"
            "    float distance = length(light.position - fragPos);\n"
            "    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
            "\n"
            "    diffuse *= attenuation;\n"
            "    specular *= attenuation;\n"
            "    ambient *= attenuation;\n"
            "\n"
            "    return (ambient + diffuse + specular);\n"
            "}\n"
            "\n"
            "void main() \n"
            "{\n"
            "    vec3 viewDir = normalize(AZIMUTH_VIEW_POS - FragPos);\n"
            "\n"
            "    vec3 result = CalcDirLight(viewDir);\n"
            "\n"
            "    for (int i = 0; i < AZIMUTH_NUM_POINT_LIGHTS; ++i)\n"
            "    {\n"
            "        result += CalcPointLight(AZIMUTH_POINT_LIGHTS[i], FragPos, viewDir);\n"
            "    };\n"
            "\n"
            "    result *= u_HDR;\n"
            "\n"
            "    FragColor = vec4(result, 1.0f);\n"
            "}\n";

        std::ofstream fileStream(directory / (name + ".glsl"));
        if (fileStream.is_open())
        {
            fileStream << file;
            fileStream.close();
        }
        else
        {
            std::cerr << "Failed to create shader file" << std::endl;
            return;
        }
    }

    void FileGenerator::GenerateGLSLFileTexturedLit(const std::filesystem::path &directory, const std::string &name)
    {
        if (std::filesystem::exists(directory / (name + ".glsl")))
        {
            std::cerr << "Error: " << name << ".glsl already exists." << std::endl;
            return;
        }

        std::string file =
            "===== VERTEX SHADER =====\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aNormal;\n"
            "layout (location = 2) in vec2 aTexCoords;\n"
            "\n"
            "out vec2 TexCoords;\n"
            "out vec3 FragPos;\n"
            "out vec3 Normal;\n"
            "\n"
            "#define AZIMUTH_MVP_UNIFORMS\n"
            "\n"
            "void main()\n"
            "{\n"
            "    TexCoords = aTexCoords;\n"
            "    FragPos = AZIMUTH_FRAG;\n"
            "    Normal = AZIMUTH_NORMAL;\n"
            "    gl_Position = AZIMUTH_POSITION;\n"
            "}\n"
            "===== FRAGMENT SHADER =====\n"
            "out vec4 FragColor;\n"
            "\n"
            "in vec2 TexCoords;\n"
            "in vec3 FragPos;\n"
            "in vec3 Normal;\n"
            "\n"
            "uniform vec3 u_Color;\n"
            "uniform float u_SpecularAmount;\n"
            "uniform float u_HDR;\n"
            "\n"
            "uniform sampler2D texture_diffuse;\n"
            "\n"
            "#define AZIMUTH_LIT_PROPERTIES\n"
            "\n"
            "vec3 CalcDirLight(vec3 normal, vec3 viewDir) {\n"
            "    vec3 lightDir = normalize(-AZIMUTH_DIR_LIGHT.direction);\n"
            "    float diff = max(dot(normal, lightDir), 0.0);\n"
            "\n"
            "    vec3 reflectDir = reflect(-lightDir, normal);\n"
            "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_SpecularAmount);\n"
            "\n"
            "    vec3 ambient = AZIMUTH_DIR_LIGHT.ambient * texture(texture_diffuse, TexCoords).rgb;\n"
            "    vec3 diffuse = AZIMUTH_DIR_LIGHT.diffuse * diff * texture(texture_diffuse, TexCoords).rgb;\n"
            "    vec3 specular = AZIMUTH_DIR_LIGHT.diffuse * spec * texture(texture_diffuse, TexCoords).rgb;\n"
            "\n"
            "    return (ambient + diffuse + specular);\n"
            "}\n"
            "\n"
            "vec3 CalcPointLight(AZIMUTH_POINT_LIGHT light, vec3 normal, vec3 fragPos, vec3 viewDir) {\n"
            "    vec3 lightDir = normalize(light.position - fragPos);\n"
            "    float diff = max(dot(normal, lightDir), 0.0);\n"
            "\n"
            "    vec3 reflectDir = reflect(-lightDir, normal);\n"
            "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_SpecularAmount);\n"
            "\n"
            "    vec3 ambient = light.ambient * vec3(texture(texture_diffuse, TexCoords)).rgb;\n"
            "    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse, TexCoords).rgb;\n"
            "    vec3 specular = light.diffuse * spec * texture(texture_diffuse, TexCoords).rgb;\n"
            "\n"
            "    float distance = length(light.position - fragPos);\n"
            "    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
            "\n"
            "    diffuse *= attenuation;\n"
            "    specular *= attenuation;\n"
            "    ambient *= attenuation;\n"
            "\n"
            "    return (ambient + diffuse + specular);\n"
            "}\n"
            "\n"
            "void main() \n"
            "{\n"
            "    vec3 normal = texture(texture_diffuse, TexCoords).rgb;\n"
            "    vec3 viewDir = normalize(AZIMUTH_VIEW_POS - FragPos);\n"
            "\n"
            "    vec3 result = CalcDirLight(normal, viewDir);\n"
            "\n"
            "    for (int i = 0; i < AZIMUTH_NUM_POINT_LIGHTS; ++i)\n"
            "    {\n"
            "        result += CalcPointLight(AZIMUTH_POINT_LIGHTS[i], normal, FragPos, viewDir);\n"
            "    };\n"
            "\n"
            "    result *= u_HDR;\n"
            "\n"
            "    FragColor = vec4(result, 1.0f);\n"
            "}\n";

        std::ofstream fileStream(directory / (name + ".glsl"));
        if (fileStream.is_open())
        {
            fileStream << file;
            fileStream.close();
        }
        else
        {
            std::cerr << "Failed to create shader file" << std::endl;
            return;
        }
    }
}