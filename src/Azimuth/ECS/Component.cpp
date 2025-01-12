#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    void ScriptsComponent::OnStart()
    {
        for (auto &script : m_Scripts)
            script->OnStart();
    }

    void ScriptsComponent::OnUpdate()
    {
        for (auto &script : m_Scripts)
            script->OnUpdate();
    }

    MeshComponent::MeshComponent()
    {
    }
    MeshComponent::~MeshComponent()
    {
    }

    void MeshComponent::CreateMesh(const std::vector<float> &verts, std::shared_ptr<Shader> shader)
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

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

    void MeshComponent::DrawMesh()
    {
        if (!shader)
            return;

        shader->use();
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}