#include <Azimuth/Scripts/ScriptSystem.h>
#include <Azimuth/ECS/Components/ScriptContainerComponent.h>
// temp
#include <Azimuth/Scripts/MonoScript.h>

namespace Azimuth
{
    void ScriptSystem::Init(ECSManager *ECS)
    {
        this->ECS = ECS;
    }

    void ScriptSystem::OnStart()
    {
        // INIT

        std::wstring dllPath = (std::filesystem::current_path() / L"libEntityScripts.dll").wstring();

        HMODULE hModule = LoadLibraryW(dllPath.c_str());
        if (!hModule)
        {
            std::cerr << "Failed to load " << dllPath.c_str() << std::endl;
            return;
        }

        auto GetModuleInstance = (ScriptModuleLoader * (*)()) GetProcAddress(hModule, "GetModule");
        if (!GetModuleInstance)
        {
            std::cerr << "Failed to locate GetModule in " << dllPath.c_str() << std::endl;
            FreeLibrary(hModule);
            hModule = nullptr;
        }

        ScriptModuleLoader *instance = GetModuleInstance();
        if (!instance)
        {
            print("No ScriptModule DLL Found");
            return;
        }

        instance->Init();
        std::vector<std::shared_ptr<MonoScript>> scripts = instance->GetScripts();

        std::unordered_map<std::string, std::shared_ptr<MonoScript>> scriptPairs;

        for (const auto &script : scripts)
            scriptPairs[script->GetID()] = script;

        for (auto &entity : m_Entities)
        {
            ScriptContainerComponent &scriptContainer = ECS->GetComponent<ScriptContainerComponent>(entity);
            scriptContainer.GetScriptIDs();

            for (auto const id : scriptContainer.GetScriptIDs())
            {
                if (scriptPairs.find(id) != scriptPairs.end())
                {
                    auto copy = scriptPairs.at(id)->Clone();
                    print(copy->GetID());
                    scriptContainer.AddScriptComponent(copy);
                }
            }
        }
    }

    void ScriptSystem::OnUpdate()
    {
        for (auto &entity : m_Entities)
        {
            ECS->GetComponent<ScriptContainerComponent>(entity).OnUpdate();
        };
    }
}