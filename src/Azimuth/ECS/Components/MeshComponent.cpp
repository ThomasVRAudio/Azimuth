#include <Azimuth/ECS/Components/MeshComponent.h>

namespace Azimuth
{

    void MeshComponent::CreateMesh(GEOMETRY_TYPE geometry)
    {
        SetMeshGeometry(geometry);
    }

    void MeshComponent::SetMeshGeometry(GEOMETRY_TYPE geometry)
    {
        switch (geometry)
        {
        case None:
            m_geometryMesh;
            break;
        case GEOMETRY_POINT:
            m_geometryMesh = Geometry::Point();
            break;
        case GEOMETRY_LINE:
            m_geometryMesh = Geometry::Line();
            break;
        case GEOMETRY_TRIANGLE:
            m_geometryMesh = Geometry::Triangle();
            break;
        case GEOMETRY_PLANE:
            m_geometryMesh = Geometry::Square();
            break;
        case GEOMETRY_CUBE:
            m_geometryMesh = Geometry::Cube();
            break;
        default:
            print("Geometry not yet implemented.");
        }

        m_Type = geometry;

        error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL error Set Mesh Buffer Data Error: " << error << std::endl;
        }
    }

    void MeshComponent::UpdateMeshGeometry(GEOMETRY_TYPE geometry)
    {
        if (geometry == m_Type)
            return;

        SetMeshGeometry(geometry);
    }

    void MeshComponent::CreateMesh(std::vector<Vertex> &vertices)
    {
    }

    void MeshComponent::DrawMesh()
    {
        if (m_Type == None)
            return;

        glBindVertexArray(m_geometryMesh.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_geometryMesh.EBO);

        GLenum geometryType = GL_TRIANGLES;
        if (m_Type == GEOMETRY_POINT)
            geometryType = GL_POINTS;
        else if (m_Type == GEOMETRY_LINE)
            geometryType = GL_LINES;

        glDrawElements(geometryType, m_geometryMesh.indices.size(), GL_UNSIGNED_INT, 0);
    }
}