#pragma once
#include <Azimuth/Common.h>
#include <dependencies/stb_image.h>

namespace Azimuth
{
    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path;
        unsigned int slot;
    };

    class TextureLoader
    {
    private:
        struct TextureLoadData
        {
            unsigned char *data;
            int width, height, nrComponents;
            bool isNormal = false;
        };

    public:
        static unsigned int LoadTexture(const std::string &path, bool isNormal = false, bool flip = true);
        static std::vector<unsigned int> LoadTextures(std::vector<std::string> &paths);
        static std::vector<unsigned int> LoadTextures(std::vector<std::filesystem::path> &paths);

    private:
        static std::vector<TextureLoadData> LoadTextureDataAsync(std::vector<std::string> &paths);
        static unsigned int GenerateTexture(const TextureLoadData &textureData);
    };
}