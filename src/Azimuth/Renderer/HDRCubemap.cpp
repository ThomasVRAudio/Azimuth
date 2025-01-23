#include <Azimuth/Renderer/HDRCubemap.h>

namespace Azimuth
{

    void HDRCubemap::LoadHDRCubemap(std::string path, unsigned int resolution)
    {
        m_Resolution = resolution;
        m_BackgroundShader = new Shader("assets/shaders/IBL/background.vert", "assets/shaders/IBL/background.frag");
        m_EquirectangularToCubemapShader = new Shader("assets/shaders/IBL/cubemap.vert", "assets/shaders/IBL/equirectangular_to_cubemap.frag");
        m_IrradianceShader = new Shader("assets/shaders/IBL/cubemap.vert", "assets/shaders/IBL/irradiance_convolution.frag");

        m_BackgroundShader->use();
        m_BackgroundShader->setInt("environmentMap", 0);

        glGenFramebuffers(1, &m_CaptureFBO);
        glGenRenderbuffers(1, &m_CaptureRBO);

        unsigned int hdrTexture = LoadHDRTexture(path);

        EquirectangularToCubemap(path, hdrTexture);
        CreateIrradianceMap();
        ResetViewport();
    }

    void HDRCubemap::EquirectangularToCubemap(std::string &path, unsigned int hdrTexture)
    {
        glGenTextures(1, &m_EnvCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, m_Resolution, m_Resolution, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        m_EquirectangularToCubemapShader->use();
        m_EquirectangularToCubemapShader->setInt("equirectangularMap", 0);
        m_EquirectangularToCubemapShader->setMat4("projection", m_CaptureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);

        glViewport(0, 0, m_Resolution, m_Resolution);
        glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_EquirectangularToCubemapShader->setMat4("view", m_CaptureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            RenderProjectionCube();
        }

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL EquirectangularToCubemap error: " << error << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void HDRCubemap::RenderProjectionCube()
    {
        if (m_VAO == 0)
        {

            PrimitiveMesh mesh;
            Mesh cubeMesh = mesh.CubeNonIndexed();

            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);

            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

            glBufferData(GL_ARRAY_BUFFER, cubeMesh.positions.size() * sizeof(float), cubeMesh.positions.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                std::cout << "OpenGL Render Projection Cube Error: " << error << std::endl;
            }
        }

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void HDRCubemap::CreateIrradianceMap()
    {
        glGenTextures(1, &m_IrradianceCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

        m_IrradianceShader->use();
        m_IrradianceShader->setInt("environmentMap", 0);
        m_IrradianceShader->setMat4("projection", m_CaptureProjection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap);

        glViewport(0, 0, 32, 32);
        glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_IrradianceShader->setMat4("view", m_CaptureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            RenderProjectionCube();
        }

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL Create Irradiance Map Error: " << error << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void HDRCubemap::DrawHDRCubemap(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        m_BackgroundShader->use();
        m_BackgroundShader->setMat4("view", viewMatrix);
        m_BackgroundShader->setMat4("projection", projectionMatrix);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap);

        RenderProjectionCube();
    }

    unsigned int HDRCubemap::LoadHDRTexture(std::string &path)
    {
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrComponents;
        float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
        unsigned int hdrTexture;
        if (data)
        {
            glGenTextures(1, &hdrTexture);
            glBindTexture(GL_TEXTURE_2D, hdrTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "OpenGL Failed to load HDR image." << std::endl;
        }

        return hdrTexture;
    }

    void HDRCubemap::ResetViewport()
    {
        int scrWidth, scrHeight;
        glfwGetFramebufferSize(Window::GetMainWindow(), &scrWidth, &scrHeight);
        glViewport(0, 0, scrWidth, scrHeight);
    }
}
