#include <Azimuth/Scripts/ScriptSystem.h>
#include <Azimuth/ECS/Components/ScriptComponent.h>

namespace Azimuth
{
    void ScriptSystem::Init(ECSManager *ECS)
    {
        this->ECS = ECS;
    }

    void ScriptSystem::OnStart()
    {
        for (auto &entity : m_Entities)
        {
            ECS->GetComponent<ScriptComponent>(entity).OnStart();
        }

        // TESTING

        std::string dllPath = (std::filesystem::current_path() / "libEntityScripts.dll").string();

        HMODULE hModule = LoadLibrary(dllPath.c_str());
        if (!hModule)
        {
            std::cerr << "Failed to load " << dllPath << std::endl;
            return;
        }

        auto GetModuleInstance = (ScriptModuleLoader * (*)()) GetProcAddress(hModule, "GetModule");
        if (!GetModuleInstance)
        {
            std::cerr << "Failed to locate GetModule in " << dllPath << std::endl;
            FreeLibrary(hModule);
            hModule = nullptr;
        }

        ScriptModuleLoader *instance = GetModuleInstance();
        if (instance)
        {
            instance->Init();
            std::vector<TestScript *> scripts = instance->GetScripts();

            for (TestScript *script : scripts)
            {
                script->OnStart();
                script->OnUpdate();
            }
        }
    }

    void ScriptSystem::OnUpdate()
    {
        for (auto &entity : m_Entities)
        {
            ECS->GetComponent<ScriptComponent>(entity).OnUpdate();
        };
    }
}