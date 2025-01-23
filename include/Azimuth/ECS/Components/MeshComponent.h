#pragma once
#include <Azimuth/ECS/Components/IComponent.h>
#include <Azimuth/Renderer/PrimitiveMeshes.h>
#include <Azimuth/Renderer/Shader.h>

namespace Azimuth
{

    class MeshComponent : public IComponent
    {
    public:
        MeshComponent()
        {
            GenerateBuffers();
            BindBuffers();
        }
        void CreateMesh(PRIMITIVE_TYPE primitive);
        void CreateMesh(const std::vector<float> &verts);
        void UpdateMeshPrimitive(PRIMITIVE_TYPE primitive);
        void DrawMesh();

        inline PRIMITIVE_TYPE GetMeshType() { return m_Type; };

    private:
        void GenerateBuffers();
        void BindBuffers();
        void SetBufferData(PRIMITIVE_TYPE primitive);
        PRIMITIVE_TYPE m_Type = None;
        unsigned int m_VAO, m_VBO, m_EBO;
        std::vector<float> m_Vertices;
        std::vector<int> m_Indices;
        GLenum error;
    };
}