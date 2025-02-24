#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    class MonoScript;

    class ScriptModule
    {
    public:
        virtual void Init() = 0;
        std::vector<std::shared_ptr<MonoScript>> GetScripts();
        virtual ~ScriptModule() = default;

    protected:
        std::vector<std::shared_ptr<MonoScript>> m_Scripts;
    };
}
