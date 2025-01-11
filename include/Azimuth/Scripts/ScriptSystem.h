#pragma once
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>

namespace Azimuth
{
    class ScriptSystem : public System
    {
    public:
        ScriptSystem() = default;
        void Init();
        void OnStart();
        void OnUpdate();
    };
}