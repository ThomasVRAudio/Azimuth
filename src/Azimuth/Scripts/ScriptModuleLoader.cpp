#include <Azimuth/Scripts/ScriptModuleLoader.h>
#include <Azimuth/ECS/Components/ScriptContainerComponent.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Scripts/ScriptModule.h>
#include <Azimuth/Scripts/MonoScript.h>

namespace Azimuth
{

    void ScriptModuleLoader::LoadModule()
    {
        if (hModule != nullptr)
            UnloadModule();

        std::wstring dllPath = (std::filesystem::current_path() / L"libEntityScripts.dll").wstring();

        hModule = LoadLibraryW(dllPath.c_str());
        if (!hModule)
        {
            std::cerr << "Failed to load " << dllPath.c_str() << std::endl;
            return;
        }

        auto GetModuleInstance = (ScriptModule * (*)()) GetProcAddress(hModule, "GetModule");
        if (!GetModuleInstance)
        {
            std::cerr << "Failed to locate GetModule in " << dllPath.c_str() << std::endl;
            UnloadModule();
        }

        ScriptModule *instance = GetModuleInstance();
        if (!instance)
        {
            print("No ScriptModule DLL Found");
            return;
        }

        instance->Init();
        std::vector<std::shared_ptr<MonoScript>> scripts = instance->GetScripts();

        for (const auto &script : scripts)
            m_ScriptPairs[script->GetPath().filename().string()] = script;

        AttachScriptComponents();
    }

    void ScriptModuleLoader::AttachScriptComponents()
    {
        Scene *scene = Application::GetActiveScene();
        ECSManager *ECS = scene->GetECSManager();
        for (auto &entity : scene->GetSceneEntities())
        {
            ScriptContainerComponent &scriptContainer = ECS->GetComponent<ScriptContainerComponent>(entity);
            scriptContainer.GetScriptPaths();

            for (auto const path : scriptContainer.GetScriptPaths())
            {
                if (m_ScriptPairs.find(path.string()) != m_ScriptPairs.end())
                {
                    auto copy = m_ScriptPairs.at(path.string())->Clone();
                    copy->SetParent(&scriptContainer);
                    scriptContainer.AddScriptComponent(copy);
                }
            }
        }
    }

    void ScriptModuleLoader::UnloadModule()
    {

        m_ScriptPairs.clear();
        FreeLibrary(hModule);
        hModule = nullptr;

        if (GetModuleHandleW(L"libEntityScripts.dll") != nullptr)
            print("DLL still unloaded.");
    }
}