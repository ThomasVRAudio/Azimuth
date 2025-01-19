#pragma once
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/RenderSystem.h>

namespace Azimuth
{
    class GameModeLayer : public Layer
    {
    public:
        virtual void Init();
        void OnStart() override;
        void OnUpdate() override;

    private:
        std::shared_ptr<RenderSystem> m_RenderSystem;
        ImVec4 m_ClearColor = ImVec4(0.7f, 0.7f, 0.9f, 1.0f);
        unsigned int m_FrameBuffer, m_SceneTexture;
        unsigned int m_SceneTextureWidth = 3840;
        Camera m_MainCamera;
    };
}