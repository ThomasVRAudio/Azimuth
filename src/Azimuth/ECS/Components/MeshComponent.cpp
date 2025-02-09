#include <Azimuth/ECS/Components/MeshComponent.h>

namespace Azimuth
{

    void MeshComponent::SetMeshGeometry(GEOMETRY_TYPE geometry)
    {
        switch (geometry)
        {
        case None:
            break;
        case GEOMETRY_POINT:
            m_Mesh = Geometry::Point();
            break;
        case GEOMETRY_LINE:
            m_Mesh = Geometry::Line();
            break;
        case GEOMETRY_TRIANGLE:
            m_Mesh = Geometry::Triangle();
            break;
        case GEOMETRY_PLANE:
            m_Mesh = Geometry::Quad();
            break;
        case GEOMETRY_CUBE:
            m_Mesh = Geometry::Cube();
            break;
        default:
            print("Geometry not yet implemented.");
        }

        m_GeometryType = geometry;

        error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL error Set Mesh Buffer Data Error: " << error << std::endl;
        }
    }

    void MeshComponent::UpdateMeshGeometry(GEOMETRY_TYPE geometry)
    {
        if (geometry == m_GeometryType)
            return;

        m_Model = nullptr;
        SetMeshGeometry(geometry);
    }

    void MeshComponent::UpdateMeshModel(std::shared_ptr<Model> model)
    {
        m_Model = model;
        SetMeshGeometry(None);
    }

    void MeshComponent::DrawMesh(Shader *shader)
    {
        if (m_Model != nullptr && shader != nullptr)
        {
            m_Model->Draw(*shader);
            return;
        }
        else if (m_GeometryType == None)
        {
            return;
        }

        glBindVertexArray(m_Mesh.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Mesh.EBO);

        GLenum geometryType = GL_TRIANGLES;
        if (m_GeometryType == GEOMETRY_POINT)
            geometryType = GL_POINTS;
        else if (m_GeometryType == GEOMETRY_LINE)
            geometryType = GL_LINES;

        glDrawElements(geometryType, m_Mesh.indices.size(), GL_UNSIGNED_INT, 0);
    }
}