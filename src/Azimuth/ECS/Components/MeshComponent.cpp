#include <Azimuth/ECS/Components/MeshComponent.h>

namespace Azimuth
{

    void MeshComponent::CreateMesh(PRIMITIVE_TYPE primitive, std::shared_ptr<Shader> shader)
    {
        GenerateBuffers();
        SetBufferData(primitive);

        if (shader)
        {
            this->shader = shader;
        }
        else
        {
            this->shader = std::make_shared<Shader>("assets/shaders/default/solid.vert", "assets/shaders/default/solid.frag");
        }
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

        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(int), m_Indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL error: " << error << std::endl;
        }
    }

    void MeshComponent::UpdateMeshPrimitive(PRIMITIVE_TYPE primitive)
    {
        if (primitive == m_Type)
            return;

        if (shader == nullptr)
        {
            CreateMesh(primitive);
            return;
        }

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

        SetBufferData(primitive);
    }

    void MeshComponent::CreateMesh(const std::vector<float> &verts, std::shared_ptr<Shader> shader)
    {
        GenerateBuffers();

        m_Vertices = verts;
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL error: " << error << std::endl;
        }

        this->shader = shader;
    }

    void MeshComponent::GenerateBuffers()
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    }

    void MeshComponent::DrawMesh()
    {
        if (!shader)
            return;

        shader->use();
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    }
}