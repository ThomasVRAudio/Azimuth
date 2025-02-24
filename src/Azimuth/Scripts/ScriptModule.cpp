#include <Azimuth/Scripts/ScriptModule.h>
#include <Azimuth/Scripts/MonoScript.h>

namespace Azimuth
{
    std::vector<std::shared_ptr<MonoScript>> ScriptModule::GetScripts() { return m_Scripts; };
}