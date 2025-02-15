#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/Renderer/LightSystem.h>
#include <Azimuth/Renderer/FrameBuffer.h>
#include <Azimuth/Editor/EditorManager.h>
#include <Azimuth/Editor/EditorCamera.h>
#include <Azimuth/Core/Time.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Events/MouseEvents.h>

namespace Azimuth
{
    class Scene;

    class EditorLayer : public Layer
    {
    public:
        virtual void Init(Scene *scene);
        void OnStart() override;
        void OnUpdate() override;
        inline void UpdateLights() { m_LightSystem->UpdateLights(); };

    private:
        std::shared_ptr<RenderSystem> m_RenderSystem;
        std::shared_ptr<LightSystem> m_LightSystem;
        std::unique_ptr<FrameBufferConfig> m_FrameBufferConfig = nullptr, m_FrameBufferEditorConfig = nullptr;
        std::shared_ptr<Shader> m_EditorShader;
        std::shared_ptr<SceneSettings> m_SceneSettings;
        ImVec4 m_ClearColor = ImVec4(0.7f, 0.7f, 0.9f, 1.0f);
        unsigned int m_FrameBuffer, m_EditorSceneTexture, m_EditorIDTexture;
        unsigned int m_EditorSceneTextureWidth = 3840;
        EditorCamera m_EditorCamera;
    };
}

#endif