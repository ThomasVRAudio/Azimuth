#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    class MonoScript;
    class ScriptModuleLoader
    {
    public:
        static void LoadModule();
        static void UnloadModule();

    private:
        inline static HMODULE hModule;
        inline static std::unordered_map<std::string, std::shared_ptr<MonoScript>> m_ScriptPairs;
        static void AttachScriptComponents();
        static void DetachScriptComponents();
    };
}