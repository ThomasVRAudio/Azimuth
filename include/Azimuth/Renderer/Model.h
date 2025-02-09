#pragma once
#include <Azimuth/Renderer/Mesh.h>
#include <Azimuth/Renderer/Shader.h>
#include <dependencies/assimp/Importer.hpp>
#include <dependencies/assimp/scene.h>
#include <dependencies/assimp/postprocess.h>
#include <dependencies/stb_image.h>
#include <Azimuth/Common.h>

namespace Azimuth
{
    class Model
    {
    public:
        std::vector<Mesh> meshes;
        Model(const char *path)
        {
            LoadModel(path);
        }
        void Draw(Shader &shader);

    private:
        std::vector<Texture> textures_loaded;
        std::string directory;

        void LoadModel(std::string path);
        void ProcessNode(aiNode *node, const aiScene *scene);
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        unsigned int TextureFromFile(const char *path, const std::string &directory, bool isNormal);
    };
}