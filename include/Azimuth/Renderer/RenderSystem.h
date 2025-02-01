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

namespace Azimuth
{
    struct Light;
    class LightSystem;

    class RenderSystem : public System
    {
    public:
        RenderSystem() = default;
        void Init(ECSManager *ECS, std::shared_ptr<LightSystem> lightSystem);
        void RenderScene(Camera &camera, unsigned int framebuffer);
        void RenderEditorPass(Camera &camera, unsigned int framebuffer, Shader *shader, unsigned int texture);

    private:
        void RenderLights(std::shared_ptr<Shader> shader, Camera &camera);
        void RenderPass(Camera &camera);
        ECSManager *m_ECS;
        glm::mat4 m_Model = glm::mat4(1.0f);
        glm::mat4 m_Projection = glm::mat4(1.0f);
        std::shared_ptr<LightSystem> m_LightSystem;
    };
}