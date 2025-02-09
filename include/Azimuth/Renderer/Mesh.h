#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Shader.h>

namespace Azimuth
{
    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    class Mesh
    {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh() {};
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        unsigned int VAO, VBO, EBO;
        void SetupMeshBuffers();
        void Draw(Shader &shader);
    };
}