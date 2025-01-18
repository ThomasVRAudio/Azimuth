#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/Renderer/FrameBuffer.h>
#include <Azimuth/Editor/EditorUI.h>
#include <Azimuth/Renderer/Camera.h>
#include <Azimuth/Core/Time.h>

namespace Azimuth
{

    class EditorLayer : public Layer
    {
    public:
        virtual void Init();
        void OnStart() override;
        void OnUpdate() override;
        void ProcessInput(GLFWwindow *window);
        void MouseCallback(GLFWwindow *window, double xposIn, double yposIn);
        void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    private:
        std::shared_ptr<RenderSystem> m_RenderSystem;
        ImVec4 m_ClearColor = ImVec4(0.7f, 0.7f, 0.9f, 1.0f);
        unsigned int m_FrameBuffer, m_EditorSceneTexture;
        unsigned int m_EditorSceneTextureWidth = 3840;
        Camera m_EditorCamera;
    };
}