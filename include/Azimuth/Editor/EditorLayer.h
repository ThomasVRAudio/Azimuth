#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Core/Layer.h>
#include <Azimuth/Editor/EditorCamera.h>
#include <Azimuth/Renderer/LightSystem.h>
#include <Azimuth/Renderer/FrameBuffer.h>

namespace Azimuth
{
    class Scene;
    class EditorCamera;
    class SceneSettings;
    class RenderSystem;
    class ShaderRecompileSystem;
    class Shader;

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
        std::shared_ptr<ShaderRecompileSystem> m_ShaderRecompileSystem;
        std::unique_ptr<FrameBufferConfig> m_FrameBufferConfig = nullptr, m_FrameBufferEditorConfig = nullptr;
        std::shared_ptr<Shader> m_EditorShader;
        std::shared_ptr<SceneSettings> m_SceneSettings;
        ImVec4 m_ClearColor = ImVec4(0.7f, 0.7f, 0.9f, 1.0f);
        unsigned int m_FrameBuffer, m_EditorSceneTexture, m_EditorIDTexture;
        unsigned int m_EditorSceneTextureWidth = 3840;
        EditorCamera m_EditorCamera;
        float m_Time;
    };
}

#endif