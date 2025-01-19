#pragma once
#include <Azimuth/Common.h>
#include <dependencies/stb_image.h>
#include <Azimuth/Renderer/Shader.h>
#include <Azimuth/Renderer/PrimitiveMeshes.h>

namespace Azimuth
{

    class Cubemap
    {
    public:
        static unsigned int LoadCubemap(std::vector<std::string> &faces);
        static void DrawCubemap(unsigned int cubemapId, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
        ~Cubemap()
        {
            if (m_CubemapShader)
                delete m_CubemapShader;
        }

    private:
        inline static Shader *m_CubemapShader;
        inline static unsigned int m_VAO, m_VBO;
        inline static bool m_isInitialized = false;
    };
}