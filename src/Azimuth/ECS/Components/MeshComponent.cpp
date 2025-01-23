#include <Azimuth/ECS/Components/MeshComponent.h>

namespace Azimuth
{

    void MeshComponent::CreateMesh(PRIMITIVE_TYPE primitive)
    {
        SetBufferData(primitive);
    }

    void MeshComponent::SetBufferData(PRIMITIVE_TYPE primitive)
    {
        PrimitiveMesh mesh;
        switch (primitive)
        {
        case None:
            m_Vertices = std::vector<float>();
            m_Indices = std::vector<int>();
            break;
        case PRIMITIVE_POINT:
            m_Vertices = mesh.Point().positions;
            m_Indices = mesh.Point().indices;
            break;
        case PRIMITIVE_LINE:
            m_Vertices = mesh.Line().positions;
            m_Indices = mesh.Line().indices;
            break;
        case PRIMITIVE_TRIANGLE:
            m_Vertices = mesh.Triangle().positions;
            m_Indices = mesh.Triangle().indices;
            break;
        case PRIMITIVE_SQUARE:
            m_Vertices = mesh.Square().positions;
            m_Indices = mesh.Square().indices;
            break;
        default:
            print("Shape not yet implemented.");
        }

        m_Type = primitive;
        BindBuffers();

        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(int), m_Indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL error Set Mesh Buffer Data Error: " << error << std::endl;
        }
    }

    void MeshComponent::UpdateMeshPrimitive(PRIMITIVE_TYPE primitive)
    {
        if (primitive == m_Type)
            return;

        SetBufferData(primitive);
    }

    void MeshComponent::CreateMesh(const std::vector<float> &verts)
    {
        BindBuffers();

        m_Vertices = verts;
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL Create Mesh Error: " << error << std::endl;
        }
    }

    void MeshComponent::GenerateBuffers()
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
    }

    void MeshComponent::BindBuffers()
    {

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL Bind Buffers Error: " << error << std::endl;
        }
    }

    void MeshComponent::DrawMesh()
    {
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    }
}