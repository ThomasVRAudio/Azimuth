#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Scripts/ScriptSystem.h>

namespace Azimuth
{

    class ScriptLayer : public Layer
    {
    public:
        virtual void Init();
        void OnStart() override;
        void OnUpdate() override;

    private:
        std::shared_ptr<ScriptSystem> m_ScriptSystem;
    };
}