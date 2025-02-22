#include <Azimuth/Scripts/ScriptModuleLoader.h>

namespace Azimuth
{
    std::vector<TestScript *> ScriptModuleLoader::GetScripts()
    {
        std::vector<TestScript *> scripts;
        for (const auto &script : m_Scripts)
        {
            scripts.emplace_back(script.get());
        }
        return scripts;
    }
}