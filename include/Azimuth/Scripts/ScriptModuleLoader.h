#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    class MonoScript;

    class ScriptModuleLoader
    {
    public:
        virtual void Init() = 0;
        std::vector<std::shared_ptr<MonoScript>> GetScripts();
        virtual ~ScriptModuleLoader() = default;

    protected:
        std::vector<std::shared_ptr<MonoScript>> m_Scripts;
    };
}
