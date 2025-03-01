#pragma once
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Scripts/ScriptSystem.h>
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
        Scene *m_Scene = nullptr;
        std::shared_ptr<ScriptSystem> m_ScriptSystem;
        std::shared_ptr<ScriptModuleLoader> m_ScriptModuleLoader;
    };
}