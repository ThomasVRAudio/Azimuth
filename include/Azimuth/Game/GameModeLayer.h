#pragma once
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Events/MouseEvents.h>

namespace Azimuth
{
    class Scene;

    class GameModeLayer : public Layer
    {
    public:
        virtual void Init(Scene *scene);
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