#include <Azimuth/Scripts/ScriptModuleLoader.h>
#include <Azimuth/Scripts/MonoScript.h>

namespace Azimuth
{
    std::vector<std::shared_ptr<MonoScript>> ScriptModuleLoader::GetScripts() { return m_Scripts; };
}