#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/Shader.h>
#include <dependencies/glm/gtc/matrix_transform.hpp>
#include <Azimuth/Renderer/Camera.h>
#include <Azimuth/Renderer/Cubemap.h>
#include <Azimuth/Renderer/HDRCubemap.h>
#include <Azimuth/Renderer/FrameBuffer.h>

namespace Azimuth
{
    struct Light;
    class LightSystem;

    class RenderSystem : public System
    {
    public:
        RenderSystem() = default;
        void Init(ECSManager *ECS, std::shared_ptr<LightSystem> lightSystem, int screenWidth, int screenHeight, unsigned int *outputTexture);
        void RenderScene(Camera &camera);

    private:
        void RenderLights(std::shared_ptr<Shader> shader, Camera &camera);
        void RenderPass(Camera &camera);
        unsigned int m_OutputFramebuffer, m_Texture;
        ECSManager *m_ECS;
        glm::mat4 m_Model = glm::mat4(1.0f);
        glm::mat4 m_Projection = glm::mat4(1.0f);
        std::shared_ptr<LightSystem> m_LightSystem;
    };
}