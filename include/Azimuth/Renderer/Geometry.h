#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
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
        std::vector<Vertex> vertices;
        std::vector<int> indices;
        unsigned int VAO, VBO, EBO;

        void SetupMeshBuffers()
        {
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            GLuint VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0); // Position
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal)); // Normal
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords)); // TexCoords
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Tangent)); // Tangent
            glEnableVertexAttribArray(3);

            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Bitangent)); // Bitangent
            glEnableVertexAttribArray(4);

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

            glBindVertexArray(0);
        }
    };

    class Geometry
    {
    public:
        static const Mesh &Point()
        {
            static Mesh mesh = []
            {
                Mesh m;
                if (m.vertices.empty())
                {
                    Vertex vertex = {
                        .Position = glm::vec3(0.0f),
                        .Normal = glm::vec3(0.0f),
                        .TexCoords = glm::vec2(0.0f),
                        .Tangent = glm::vec3(0.0f),
                        .Bitangent = glm::vec3(0.0f)};

                    m.vertices.emplace_back(vertex);
                    m.indices = {0};
                    m.SetupMeshBuffers();
                }
                return m;
            }();
            return mesh;
        }

        static const Mesh &Line()
        {
            static Mesh mesh = []
            {
                Mesh m;
                if (m.vertices.empty())
                {
                    m.vertices = {
                        Vertex{
                            .Position = glm::vec3(-0.5f, -0.5f, 0.0f),
                            .Normal = glm::vec3(0.0f),
                            .TexCoords = glm::vec2(0.0f),
                            .Tangent = glm::vec3(0.0f),
                            .Bitangent = glm::vec3(0.0f)},

                        Vertex{
                            .Position = glm::vec3(0.5f, 0.5f, 0.0f),
                            .Normal = glm::vec3(0.0f),
                            .TexCoords = glm::vec2(0.0f),
                            .Tangent = glm::vec3(0.0f),
                            .Bitangent = glm::vec3(0.0f)}};

                    m.indices = {0, 1};
                    m.SetupMeshBuffers();
                }
                return m;
            }();
            return mesh;
        }

        static const Mesh &Triangle()
        {
            static Mesh mesh = []
            {
                Mesh m;
                if (m.vertices.empty())
                {
                    m.vertices = {
                        Vertex{
                            .Position = glm::vec3(-0.5f, -0.5f, 0.0f),
                            .Normal = glm::vec3(0.0f),
                            .TexCoords = glm::vec2(0.0f),
                            .Tangent = glm::vec3(0.0f),
                            .Bitangent = glm::vec3(0.0f)},

                        Vertex{
                            .Position = glm::vec3(0.5f, -0.5f, 0.0f),
                            .Normal = glm::vec3(0.0f),
                            .TexCoords = glm::vec2(0.0f),
                            .Tangent = glm::vec3(0.0f),
                            .Bitangent = glm::vec3(0.0f)},

                        Vertex{
                            .Position = glm::vec3(0.0f, 0.5f, 0.0f),
                            .Normal = glm::vec3(0.0f),
                            .TexCoords = glm::vec2(0.0f),
                            .Tangent = glm::vec3(0.0f),
                            .Bitangent = glm::vec3(0.0f)}};

                    m.indices = {0, 1, 2};
                    m.SetupMeshBuffers();
                }
                return m;
            }();
            return mesh;
        }

        static const Mesh &Square()
        {
            static Mesh mesh = []
            {
                Mesh m;
                if (m.vertices.empty())
                {
                    m.vertices = {
                        Vertex{
                            .Position = glm::vec3(-0.5f, -0.5f, 0.0f),
                            .Normal = glm::vec3(0.0f),
                            .TexCoords = glm::vec2(0.0f),
                            .Tangent = glm::vec3(0.0f),
                            .Bitangent = glm::vec3(0.0f)},

                        Vertex{
                            .Position = glm::vec3(0.5f, -0.5f, 0.0f),
                            .Normal = glm::vec3(0.0f),
                            .TexCoords = glm::vec2(0.0f),
                            .Tangent = glm::vec3(0.0f),
                            .Bitangent = glm::vec3(0.0f)},

                        Vertex{
                            .Position = glm::vec3(0.5f, 0.5f, 0.0f),
                            .Normal = glm::vec3(0.0f),
                            .TexCoords = glm::vec2(0.0f),
                            .Tangent = glm::vec3(0.0f),
                            .Bitangent = glm::vec3(0.0f)},

                        Vertex{
                            .Position = glm::vec3(-0.5f, 0.5f, 0.0f),
                            .Normal = glm::vec3(0.0f),
                            .TexCoords = glm::vec2(0.0f),
                            .Tangent = glm::vec3(0.0f),
                            .Bitangent = glm::vec3(0.0f)}};

                    m.indices = {0, 1, 2, 2, 3, 0};
                    m.SetupMeshBuffers();
                }
                return m;
            }();
            return mesh;
        }

        static const Mesh &Cube()
        {
            static Mesh mesh;
            if (mesh.vertices.empty())

                mesh.vertices = {
                    // Front face
                    Vertex{.Position = glm::vec3(-0.5f, -0.5f, -0.5f), .Normal = glm::vec3(0.0f, 0.0f, -1.0f), .TexCoords = glm::vec2(0.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(0.5f, -0.5f, -0.5f), .Normal = glm::vec3(0.0f, 0.0f, -1.0f), .TexCoords = glm::vec2(1.0f, 1.0f)},
                    Vertex{.Position = glm::vec3(0.5f, 0.5f, -0.5f), .Normal = glm::vec3(0.0f, 0.0f, -1.0f), .TexCoords = glm::vec2(1.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(-0.5f, 0.5f, -0.5f), .Normal = glm::vec3(0.0f, 0.0f, -1.0f), .TexCoords = glm::vec2(1.0f, 1.0f)},

                    // Back face
                    Vertex{.Position = glm::vec3(-0.5f, -0.5f, 0.5f), .Normal = glm::vec3(0.0f, 0.0f, 1.0f), .TexCoords = glm::vec2(0.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(0.5f, -0.5f, 0.5f), .Normal = glm::vec3(0.0f, 0.0f, 1.0f), .TexCoords = glm::vec2(1.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(0.5f, 0.5f, 0.5f), .Normal = glm::vec3(0.0f, 0.0f, 1.0f), .TexCoords = glm::vec2(1.0f, 1.0f)},
                    Vertex{.Position = glm::vec3(-0.5f, 0.5f, 0.5f), .Normal = glm::vec3(0.0f, 0.0f, 1.0f), .TexCoords = glm::vec2(1.0f, 1.0f)},

                    // Left face
                    Vertex{.Position = glm::vec3(-0.5f, 0.5f, 0.5f), .Normal = glm::vec3(-1.0f, 0.0f, 0.0f), .TexCoords = glm::vec2(1.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(-0.5f, 0.5f, -0.5f), .Normal = glm::vec3(-1.0f, 0.0f, 0.0f), .TexCoords = glm::vec2(0.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(-0.5f, -0.5f, -0.5f), .Normal = glm::vec3(-1.0f, 0.0f, 0.0f), .TexCoords = glm::vec2(0.0f, 1.0f)},
                    Vertex{.Position = glm::vec3(-0.5f, -0.5f, 0.5f), .Normal = glm::vec3(-1.0f, 0.0f, 0.0f), .TexCoords = glm::vec2(0.0f, 1.0f)},

                    // Right face
                    Vertex{.Position = glm::vec3(0.5f, 0.5f, 0.5f), .Normal = glm::vec3(1.0f, 0.0f, 0.0f), .TexCoords = glm::vec2(0.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(0.5f, -0.5f, -0.5f), .Normal = glm::vec3(1.0f, 0.0f, 0.0f), .TexCoords = glm::vec2(1.0f, 1.0f)},
                    Vertex{.Position = glm::vec3(0.5f, 0.5f, -0.5f), .Normal = glm::vec3(1.0f, 0.0f, 0.0f), .TexCoords = glm::vec2(0.0f, 1.0f)},
                    Vertex{.Position = glm::vec3(0.5f, -0.5f, 0.5f), .Normal = glm::vec3(1.0f, 0.0f, 0.0f), .TexCoords = glm::vec2(1.0f, 1.0f)},

                    // Bottom face
                    Vertex{.Position = glm::vec3(-0.5f, -0.5f, -0.5f), .Normal = glm::vec3(0.0f, -1.0f, 0.0f), .TexCoords = glm::vec2(0.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(0.5f, -0.5f, -0.5f), .Normal = glm::vec3(0.0f, -1.0f, 0.0f), .TexCoords = glm::vec2(1.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(0.5f, -0.5f, 0.5f), .Normal = glm::vec3(0.0f, -1.0f, 0.0f), .TexCoords = glm::vec2(1.0f, 1.0f)},
                    Vertex{.Position = glm::vec3(-0.5f, -0.5f, 0.5f), .Normal = glm::vec3(0.0f, -1.0f, 0.0f), .TexCoords = glm::vec2(1.0f, 1.0f)},

                    // Top face
                    Vertex{.Position = glm::vec3(-0.5f, 0.5f, -0.5f), .Normal = glm::vec3(0.0f, 1.0f, 0.0f), .TexCoords = glm::vec2(1.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(0.5f, 0.5f, -0.5f), .Normal = glm::vec3(0.0f, 1.0f, 0.0f), .TexCoords = glm::vec2(0.0f, 1.0f)},
                    Vertex{.Position = glm::vec3(0.5f, 0.5f, 0.5f), .Normal = glm::vec3(0.0f, 1.0f, 0.0f), .TexCoords = glm::vec2(0.0f, 0.0f)},
                    Vertex{.Position = glm::vec3(-0.5f, 0.5f, 0.5f), .Normal = glm::vec3(0.0f, 1.0f, 0.0f), .TexCoords = glm::vec2(0.0f, 1.0f)}};

            mesh.indices = {

                // Front face
                0, 2, 1,
                0, 3, 2,

                // Back face
                4, 5, 6,
                4, 6, 7,

                // Left face
                8, 9, 10,
                8, 10, 11,

                // Right face
                12, 13, 14,
                12, 15, 13,

                // Bottom face
                16, 17, 18,
                16, 18, 19,

                // Top face
                20, 21, 22,
                20, 22, 23

            };

            mesh.SetupMeshBuffers();

            return mesh;
        }
    };
}
