#pragma once
#include <Azimuth/ECS/Components/IComponent.h>
#include <Azimuth/Renderer/Geometry.h>
#include <Azimuth/Renderer/Shader.h>

namespace Azimuth
{

    class MeshComponent : public IComponent
    {
    public:
        MeshComponent() = default;
        void CreateMesh(GEOMETRY_TYPE geometry);
        void CreateMesh(std::vector<Vertex> &vertices);
        void UpdateMeshGeometry(GEOMETRY_TYPE geometry);
        void DrawMesh();

        inline GEOMETRY_TYPE GetMeshType() { return m_Type; };

    private:
        void SetMeshGeometry(GEOMETRY_TYPE geometry);
        GEOMETRY_TYPE m_Type = None;
        Mesh m_geometryMesh;
        unsigned int m_VAO, m_VBO, m_EBO;
        std::vector<float> m_Vertices;
        std::vector<int> m_Indices;
        GLenum error;
    };
}