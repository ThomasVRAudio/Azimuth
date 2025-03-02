#include <Azimuth/Renderer/TextureLoader.h>

namespace Azimuth
{

    unsigned int TextureLoader::LoadTexture(const std::string &path, bool isNormal, bool flip, bool sRGB)
    {
        stbi_set_flip_vertically_on_load(flip);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

        return GenerateTexture(TextureLoadData{data, width, height, nrComponents, isNormal}, sRGB);
    }

    std::vector<unsigned int> TextureLoader::LoadTextures(std::vector<std::filesystem::path> &paths, bool sRGB)
    {
        std::vector<std::string> stringPaths;
        for (auto &path : paths)
            stringPaths.emplace_back(path.string());

        return LoadTextures(stringPaths, sRGB);
    }

    std::vector<unsigned int> TextureLoader::LoadTextures(std::vector<std::string> &paths, bool sRGB)
    {
        std::vector<TextureLoadData> data = LoadTextureDataAsync(paths);
        std::vector<unsigned int> textures;

        for (auto &d : data)
            textures.emplace_back(GenerateTexture(d, sRGB));

        return textures;
    }

    std::vector<TextureLoader::TextureLoadData> TextureLoader::LoadTextureDataAsync(std::vector<std::string> &paths)
    {
        stbi_set_flip_vertically_on_load(false);
        std::vector<std::future<TextureLoadData>> futures;

        for (const auto &path : paths)
        {
            futures.emplace_back(std::async(std::launch::async, [path]()
                                            {
                int width, height, nrComponents;
                unsigned char *d = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
                return TextureLoadData{d, width, height, nrComponents}; }));
        }

        std::vector<TextureLoadData> data;

        for (auto &future : futures)
            data.emplace_back(future.get());

        return data;
    }

    unsigned int TextureLoader::GenerateTexture(const TextureLoadData &textureData, bool sRGB)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        GLint prevTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTexture);

        if (textureData.data)
        {
            GLenum format;
            GLenum internalFormat;

            if (textureData.nrComponents == 1)
            {
                internalFormat = GL_RED;
                format = GL_RED;
            }
            else if (textureData.nrComponents == 3)
            {
                internalFormat = sRGB ? GL_SRGB : GL_RGB;
                format = GL_RGB;
            }
            else if (textureData.nrComponents == 4)
            {
                internalFormat = sRGB ? GL_SRGB_ALPHA : GL_RGBA;
                format = GL_RGBA;
            }
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, textureData.isNormal ? format : internalFormat, textureData.width,
                         textureData.height, 0, format, GL_UNSIGNED_BYTE, textureData.data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        stbi_image_free(textureData.data);

        glBindTexture(GL_TEXTURE_2D, prevTexture);

        return textureID;
    }
}