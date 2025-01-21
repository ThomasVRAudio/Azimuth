#include <Azimuth/Renderer/Cubemap.h>

namespace Azimuth
{

    unsigned int Cubemap::LoadCubemap(std::vector<std::string> &faces)
    {
        if (!Cubemap::m_isInitialized)
        {
            Cubemap::m_CubemapShader = new Shader("assets/shaders/cubemap.vert", "assets/shaders/cubemap.frag");

            glGenVertexArrays(1, &Cubemap::m_VAO);
            glGenBuffers(1, &Cubemap::m_VBO);

            glBindVertexArray(Cubemap::m_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, Cubemap::m_VBO);

            PrimitiveMesh mesh;
            Mesh cubeMesh = mesh.CubeNonIndexed();

            glBufferData(GL_ARRAY_BUFFER, cubeMesh.positions.size() * sizeof(float), cubeMesh.positions.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                std::cout << "OpenGL Cubemap error: " << error << std::endl;
            }

            Cubemap::m_isInitialized = true;
        }

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        unsigned char *data;

        for (unsigned int i = 0; i < faces.size(); i++)
        {
            data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                print("Cubemap failed to load at path: " << faces[i]);
            }

            stbi_image_free(data);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        };

        return textureID;
    }

    void Cubemap::DrawCubemap(unsigned int cubemapId, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        m_CubemapShader->use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));
        m_CubemapShader->setMat4("view", view);
        m_CubemapShader->setMat4("projection", projectionMatrix);

        glBindVertexArray(Cubemap::m_VAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
