#include <Azimuth/Common.h>

namespace Azimuth
{
    class Mesh
    {
    public:
        std::vector<float> positions;
        std::vector<int> indices;
    };

    class PrimitiveMesh
    {
    public:
        Mesh Point()
        {
            Mesh mesh;
            mesh.positions = {0.0f, 0.0f, 0.0f};
            mesh.indices = {0};
            return mesh;
        }

        Mesh Line()
        {
            Mesh mesh;
            mesh.positions = {-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f};
            mesh.indices = {0, 1};
            return mesh;
        }

        Mesh Triangle()
        {
            Mesh mesh;
            mesh.positions = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
            mesh.indices = {0, 1, 2};
            return mesh;
        }

        Mesh Square()
        {
            Mesh mesh;
            mesh.positions = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
            mesh.indices = {0, 1, 2, 2, 3, 0};
            return mesh;
        }
    };
}