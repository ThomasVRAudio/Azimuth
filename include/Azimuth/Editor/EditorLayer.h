#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/ECSManager.h>

namespace Azimuth
{

    class EditorLayer : public Layer
    {
    public:
        void OnStart() override;
        void OnUpdate() override;

    private:
        std::shared_ptr<RenderSystem> m_RenderSystem;
    };
}