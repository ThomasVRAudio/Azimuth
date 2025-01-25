#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/Shader.h>
#include <dependencies/glm/gtc/matrix_transform.hpp>
#include <Azimuth/Renderer/Camera.h>
#include <Azimuth/Renderer/Cubemap.h>
#include <Azimuth/Renderer/HDRCubemap.h>

namespace Azimuth
{
    struct Light;
    class RenderSystem : public System
    {
    public:
        RenderSystem() = default;
        void Init(ECSManager *ECS, std::vector<std::shared_ptr<Light>> *lights);
        void DrawScene(Camera &camera);

    private:
        ECSManager *m_ECS;
        std::vector<std::shared_ptr<Light>> *m_Lights;
        glm::mat4 m_Model = glm::mat4(1.0f);
        glm::mat4 m_Projection = glm::mat4(1.0f);
        unsigned int m_CubemapId;
    };
}