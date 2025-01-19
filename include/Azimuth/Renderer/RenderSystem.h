#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/Shader.h>
#include <dependencies/glm/gtc/matrix_transform.hpp>
#include <Azimuth/Renderer/Camera.h>
#include <Azimuth/Renderer/Cubemap.h>

namespace Azimuth
{
    class RenderSystem : public System
    {
    public:
        RenderSystem() = default;
        ~RenderSystem();
        void Init();
        void DrawScene(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    private:
        Shader *activeShader;
        unsigned int VBO, VAO;
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        unsigned int m_CubemapId;
    };
}