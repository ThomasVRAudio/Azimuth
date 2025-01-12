#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/Shader.h>
#include <dependencies/glm/gtc/matrix_transform.hpp>

namespace Azimuth
{
    /*
        Init
        BeginScene // set camera
        EndScene // Draw everything
        DrawObject // Add to things to draw

    */

    class RenderSystem : public System
    {
    public:
        RenderSystem() = default;
        ~RenderSystem();
        void Init();
        void DrawScene();

    private:
        Shader *activeShader;
        unsigned int VBO, VAO;
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
    };
}