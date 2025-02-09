#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Mesh.h>

namespace Azimuth
{
    class Geometry
    {
    public:
        static const Mesh &Point()
        {
            static Mesh mesh;
            if (mesh.vertices.empty())
            {
                Vertex vertex = {
                    .Position = glm::vec3(0.0f),
                    .Normal = glm::vec3(0.0f),
                    .TexCoords = glm::vec2(0.0f),
                    .Tangent = glm::vec3(0.0f),
                    .Bitangent = glm::vec3(0.0f)};

                mesh.vertices.emplace_back(vertex);
                mesh.indices = {0};
                mesh.SetupMeshBuffers();
            }
            return mesh;
        }

        static const Mesh &Line()
        {
            static Mesh mesh;
            if (mesh.vertices.empty())
            {
                mesh.vertices = {
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

                mesh.indices = {0, 1};
                mesh.SetupMeshBuffers();
            }
            return mesh;
        }

        static const Mesh &Triangle()
        {
            static Mesh mesh;
            if (mesh.vertices.empty())
            {
                mesh.vertices = {
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

                mesh.indices = {0, 1, 2};
                mesh.SetupMeshBuffers();
            }
            return mesh;
        }

        static const Mesh &Quad()
        {
            static Mesh mesh;
            if (mesh.vertices.empty())
            {
                mesh.vertices = {
                    Vertex{
                        .Position = glm::vec3(-0.5f, -0.5f, 0.0f),
                        .Normal = glm::vec3(0.0f),
                        .TexCoords = glm::vec2(0.0f),
                        .Tangent = glm::vec3(0.0f),
                        .Bitangent = glm::vec3(0.0f)},

                    Vertex{
                        .Position = glm::vec3(0.5f, -0.5f, 0.0f),
                        .Normal = glm::vec3(0.0f),
                        .TexCoords = glm::vec2(1.0f, 0.0f),
                        .Tangent = glm::vec3(0.0f),
                        .Bitangent = glm::vec3(0.0f)},

                    Vertex{
                        .Position = glm::vec3(0.5f, 0.5f, 0.0f),
                        .Normal = glm::vec3(0.0f),
                        .TexCoords = glm::vec2(1.0f, 1.0f),
                        .Tangent = glm::vec3(0.0f),
                        .Bitangent = glm::vec3(0.0f)},

                    Vertex{
                        .Position = glm::vec3(-0.5f, 0.5f, 0.0f),
                        .Normal = glm::vec3(0.0f),
                        .TexCoords = glm::vec2(0.0f, 1.0f),
                        .Tangent = glm::vec3(0.0f),
                        .Bitangent = glm::vec3(0.0f)}};

                mesh.indices = {0, 1, 2, 2, 3, 0};
                mesh.SetupMeshBuffers();
            }
            return mesh;
        }

        static const Mesh &Screen()
        {
            static Mesh mesh;
            if (mesh.vertices.empty())
            {
                mesh.vertices = {
                    Vertex{
                        .Position = glm::vec3(-1.0f, -1.0f, 0.0f),
                        .Normal = glm::vec3(0.0f),
                        .TexCoords = glm::vec2(0.0f),
                        .Tangent = glm::vec3(0.0f),
                        .Bitangent = glm::vec3(0.0f)},

                    Vertex{
                        .Position = glm::vec3(1.0f, -1.0f, 0.0f),
                        .Normal = glm::vec3(0.0f),
                        .TexCoords = glm::vec2(1.0f, 0.0f),
                        .Tangent = glm::vec3(0.0f),
                        .Bitangent = glm::vec3(0.0f)},

                    Vertex{
                        .Position = glm::vec3(1.0f, 1.0f, 0.0f),
                        .Normal = glm::vec3(0.0f),
                        .TexCoords = glm::vec2(1.0f, 1.0f),
                        .Tangent = glm::vec3(0.0f),
                        .Bitangent = glm::vec3(0.0f)},

                    Vertex{
                        .Position = glm::vec3(-1.0f, 1.0f, 0.0f),
                        .Normal = glm::vec3(0.0f),
                        .TexCoords = glm::vec2(0.0f, 1.0f),
                        .Tangent = glm::vec3(0.0f),
                        .Bitangent = glm::vec3(0.0f)}};

                mesh.indices = {0, 1, 2, 2, 3, 0};
                mesh.SetupMeshBuffers();
            }
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
