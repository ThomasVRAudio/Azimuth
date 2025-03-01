#pragma once
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Scripts/ScriptModule.h>
#include <Azimuth/Scripts/ScriptModuleLoader.h>

namespace Azimuth
{
    class ScriptSystem : public System
    {
    public:
        ScriptSystem() = default;
        void Init(Scene *scene);
        void OnStart();
        void OnUpdate();

    private:
        Scene *m_Scene;
    };
}