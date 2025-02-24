#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Scripts/ScriptSystem.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Scripts/ScriptModuleLoader.h>

namespace Azimuth
{
    class Scene;

    class ScriptLayer : public Layer
    {
    public:
        virtual void Init(Scene *scene);
        void OnStart() override;
        void OnUpdate() override;

    private:
        std::shared_ptr<ScriptSystem> m_ScriptSystem;
        std::shared_ptr<ScriptModuleLoader> m_ScriptModuleLoader;
    };
}