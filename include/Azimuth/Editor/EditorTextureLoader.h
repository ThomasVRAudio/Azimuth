#ifdef AZIMUTH_EDITOR
#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/TextureLoader.h>
#include <Azimuth/System/Files.h>

namespace Azimuth
{
    class EditorTextureLoader
    {
    public:
        static void LoadEditorTextures();
        static void LoadDirectoryTextures();
        static int GetTextureID(const std::string &texture);
        static int GetOrLoadTexture(const std::string &path);

    private:
        inline static std::unordered_map<std::string, unsigned int> m_TextureMap;
    };
}

#endif