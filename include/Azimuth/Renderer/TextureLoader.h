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
    public:
        static unsigned int LoadTexture(const std::string &path, bool isNormal = false);
    };
}