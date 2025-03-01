#pragma once
#include <Azimuth/ECS/Components/IComponent.h>
#include <Azimuth/Renderer/Geometry.h>
#include <Azimuth/Renderer/Shader.h>
#include <Azimuth/Renderer/Model.h>

namespace Azimuth
{

    class MeshComponent : public IComponent
    {
    public:
        MeshComponent() = default;
        MeshComponent(const MeshComponent &other)
            : m_GeometryType(other.m_GeometryType),
              m_Mesh(other.m_Mesh),
              m_VAO(other.m_VAO),
              m_VBO(other.m_VBO),
              m_EBO(other.m_EBO),
              m_Vertices(other.m_Vertices),
              m_Indices(other.m_Indices),
              error(other.error)
        {
            if (other.m_Model)
                m_Model = std::make_shared<Model>(*other.m_Model);
        }

        inline void CreateMesh(GEOMETRY_TYPE geometry) { SetMeshGeometry(geometry); }
        inline void CreateMesh(std::shared_ptr<Model> model) { m_Model = model; }
        void UpdateMeshGeometry(GEOMETRY_TYPE geometry);
        void UpdateMeshModel(std::shared_ptr<Model> model);
        void DrawMesh(Shader *shader = nullptr);
        inline GEOMETRY_TYPE GetMeshType() { return m_GeometryType; };

    private:
        void SetMeshGeometry(GEOMETRY_TYPE geometry);
        GEOMETRY_TYPE m_GeometryType = None;
        Mesh m_Mesh;
        std::shared_ptr<Model> m_Model = nullptr;
        unsigned int m_VAO, m_VBO, m_EBO;
        std::vector<float> m_Vertices;
        std::vector<int> m_Indices;
        GLenum error;
        friend class Serializer;
    };
}