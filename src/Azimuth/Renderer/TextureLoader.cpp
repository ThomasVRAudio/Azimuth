#include <Azimuth/Renderer/TextureLoader.h>

namespace Azimuth
{
    unsigned int TextureLoader::LoadTexture(const std::string &path, bool isNormal)
    {

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

        unsigned int textureID;
        glGenTextures(1, &textureID);

        GLint prevTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTexture);

        if (data)
        {
            GLenum format;
            GLenum internalFormat;

            if (nrComponents == 1)
            {
                internalFormat = GL_RED;
                format = GL_RED;
            }
            else if (nrComponents == 3)
            {
                internalFormat = GL_SRGB;
                format = GL_RGB;
            }
            else if (nrComponents == 4)
            {
                internalFormat = GL_SRGB_ALPHA;
                format = GL_RGBA;
            }
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, isNormal ? format : internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        glBindTexture(GL_TEXTURE_2D, prevTexture);

        return textureID;
    }
}