#include <Azimuth/Project/FileGenerator.h>

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

        UpdateDLLExportFile(directory, name);
    }

    void FileGenerator::UpdateDLLExportFile(std::filesystem::path path, const std::string &name)
    {
        std::filesystem::path targetPath = Application::projectSettings->ProjectFolder / "azimuth" / "dllexport.h";
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
                std::filesystem::path relative = std::filesystem::relative(path, Application::projectSettings->ProjectFolder);

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

    void FileGenerator::GenerateProjectFiles()
    {
        GenerateDLLExportFiles();
        GenerateCMakeFile();
    }

    void FileGenerator::GenerateDLLExportFiles()
    {
        std::filesystem::path targetDirectory = Application::projectSettings->ProjectFolder / "azimuth";
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
            "    class DLLExport : public ScriptModuleLoader\n"
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

    void FileGenerator::GenerateCMakeFile()
    {

        std::filesystem::path targetPath = Application::projectSettings->ProjectFolder / "CMakeLists.txt";
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
            "    RUNTIME_OUTPUT_DIRECTORY ${AZIMUTH_ENGINE_PATH}/build\n"
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
}