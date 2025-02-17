#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/Shader.h>
#include <dependencies/glm/gtc/matrix_transform.hpp>
#include <Azimuth/Renderer/Camera.h>
#include <Azimuth/Renderer/HDRCubemap.h>
#include <Azimuth/Renderer/FrameBuffer.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Renderer/LightSystem.h>
#include <Azimuth/Scene/SceneSettings.h>
#include <Azimuth/Renderer/Bloom.h>
#include <Azimuth/Renderer/Model.h> // test

namespace Azimuth
{
    struct Light;
    class LightSystem;
#ifdef AZIMUTH_EDITOR
    class EditorSettingsPanel;
#endif

    class RenderSystem : public System
    {
    public:
        RenderSystem() = default;
        void Init(ECSManager *ECS, std::shared_ptr<LightSystem> lightSystem, unsigned int width = 3840, unsigned int height = 2160);
        void RenderScene(Camera &camera, unsigned int outputFramebuffer, SceneSettings *settings);
        void RenderEditorPass(Camera &camera, unsigned int framebuffer, Shader *shader, unsigned int texture);

    private:
        void RenderLights(std::shared_ptr<Shader> shader, Camera &camera);
        void RenderPass(Camera &camera, SceneSettings *settings = nullptr);
        void RenderScreenQuad(Shader *shader, unsigned int texture, unsigned int activeTexture = 0);
        ECSManager *m_ECS;
        glm::mat4 m_Model = glm::mat4(1.0f);
        glm::mat4 m_Projection = glm::mat4(1.0f);
        std::shared_ptr<LightSystem> m_LightSystem;
        std::unique_ptr<FrameBufferConfig> m_SceneRenderFrameBuffer;
        std::unique_ptr<FrameBufferConfig> m_TonemappingFrameBuffer;
        std::unique_ptr<FrameBufferConfig> m_PrefilterFrameBuffer;
        std::unique_ptr<Shader> m_PostProcessShader = nullptr;
        std::unique_ptr<Shader> m_FinalCompositeShader = nullptr;
        std::unique_ptr<Shader> m_ToneMappingShader = nullptr;
        std::unique_ptr<Shader> m_PrefilterShader = nullptr;
        std::unique_ptr<FrameBufferConfig> m_PostProcessingFrameBuffer;
        Mesh m_RenderScreenQuad = Geometry::Screen();
        std::unique_ptr<Bloom> m_Bloom = nullptr;
        unsigned int m_RenderedSceneTexture, m_ToneMappedTexture, m_PostProcessedTexture, m_PrefilteredTexture;
    };
}