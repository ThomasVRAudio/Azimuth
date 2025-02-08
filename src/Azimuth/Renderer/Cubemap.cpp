#include <Azimuth/Renderer/Cubemap.h>

namespace Azimuth
{

    unsigned int Cubemap::LoadCubemap(std::vector<std::string> &faces)
    {
        Cubemap::m_CubemapShader = new Shader("assets/shaders/system/cubemap/cubemap.vert", "assets/shaders/system/cubemap/cubemap.frag");

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

        Mesh cubeMesh = Geometry::Cube();

        glBindVertexArray(cubeMesh.VAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);

        glDrawElements(GL_TRIANGLES, cubeMesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
