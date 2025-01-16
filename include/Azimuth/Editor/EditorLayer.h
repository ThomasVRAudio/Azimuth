#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/RenderSystem.h>

#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Renderer/FrameBuffer.h>
#include <Azimuth/Editor/ImGuiStyling.h>

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
        ImGuiWindowFlags m_WindowFlags;
        ImVec4 m_ClearColor = ImVec4(0.4f, 0.2f, 0.3f, 1.0f); // test
        unsigned int frameBuffer, texture;
        unsigned int width = 800, height = 600;
    };
}