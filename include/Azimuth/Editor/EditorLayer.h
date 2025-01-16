#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/Renderer/FrameBuffer.h>
#include <Azimuth/Editor/EditorUI.h>

namespace Azimuth
{

    class EditorLayer : public Layer
    {
    public:
        virtual void Init();
        void OnStart() override;
        void OnUpdate() override;

    private:
        std::shared_ptr<RenderSystem> m_RenderSystem;
        ImVec4 m_ClearColor = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
        unsigned int m_FrameBuffer, m_EditorSceneTexture;
        unsigned int m_EditorSceneTextureWidth = 3840;
    };
}