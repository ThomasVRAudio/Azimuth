#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    class ScriptModuleLoader
    {
    public:
        virtual void Init() = 0;
        std::vector<TestScript *> GetScripts();
        virtual ~ScriptModuleLoader() = default;

    protected:
        std::vector<std::unique_ptr<TestScript>> m_Scripts;
    };
}
