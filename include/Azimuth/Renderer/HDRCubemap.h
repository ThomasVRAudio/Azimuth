#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Shader.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Renderer/PrimitiveMeshes.h>
#include <dependencies/stb_image.h>

namespace Azimuth
{
    class HDRCubemap
    {
    public:
        static void LoadHDRCubemap(std::string path, unsigned int resolution);
        static void DrawHDRCubemap(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
        ~HDRCubemap()
        {
            if (m_BackgroundShader)
                delete m_BackgroundShader;

            if (m_EquirectangularToCubemapShader)
                delete m_EquirectangularToCubemapShader;

            if (m_IrradianceShader)
                delete m_IrradianceShader;
        }

    private:
        static void RenderProjectionCube();
        static unsigned int LoadHDRTexture(std::string &path);
        static void EquirectangularToCubemap(std::string &path, unsigned int hdrTexture);
        static void CreateIrradianceMap();
        static void ResetViewport();
        inline static Shader *m_BackgroundShader;
        inline static Shader *m_EquirectangularToCubemapShader;
        inline static Shader *m_IrradianceShader;
        inline static unsigned int m_VAO = 0, m_VBO;
        inline static unsigned int m_CaptureFBO, m_CaptureRBO;
        inline static unsigned int m_EnvCubemap, m_IrradianceCubemap;
        inline static unsigned int m_Resolution = 512;
        inline static glm::mat4 m_CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        inline static glm::mat4 m_CaptureViews[] =
            {
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};
    };
}