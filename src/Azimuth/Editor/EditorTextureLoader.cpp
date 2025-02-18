#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorTextureLoader.h>

namespace Azimuth
{
    void EditorTextureLoader::LoadEditorTextures()
    {
        unsigned int texture = TextureLoader::LoadTexture("assets/editor/textures/folder_black.png", false, false);
        m_TextureMap["folder"] = texture;

        texture = TextureLoader::LoadTexture("assets/editor/textures/file.png", false, false);
        m_TextureMap["file"] = texture;

        texture = TextureLoader::LoadTexture("assets/editor/textures/back_button.png", false, false);
        m_TextureMap["back"] = texture;
    }

    void EditorTextureLoader::LoadDirectoryTextures()
    {
        std::vector<std::filesystem::path> textureFiles = Files::GetFilesWithExtension(Application::projectSettings->ProjectFolder.string(), {".png", ".jpg"});
        std::vector<unsigned int> textures = TextureLoader::LoadTextures(textureFiles);
        for (size_t i = 0; i < textureFiles.size(); ++i)
            m_TextureMap[textureFiles[i].string()] = textures[i];
    }

    int EditorTextureLoader::GetOrLoadTexture(const std::string &path)
    {
        unsigned int texture = GetTextureID(path);
        if (texture == -1)
        {
            texture = TextureLoader::LoadTexture(path, false, false);
            m_TextureMap[path] = texture;
        }

        return texture;
    }

    int EditorTextureLoader::GetTextureID(const std::string &texture)
    {
        auto it = m_TextureMap.find(texture);
        if (it != m_TextureMap.end())
        {
            return it->second;
        }
        else
        {
            return -1;
        }
    }
}

#endif