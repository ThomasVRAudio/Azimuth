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