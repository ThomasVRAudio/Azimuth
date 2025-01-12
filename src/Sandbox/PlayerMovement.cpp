#include "PlayerMovement.h"

void Azimuth::PlayerMovement::OnStart()
{
    AddComponent<TransformComponent>();
    AddComponent<MeshComponent>();

    MeshComponent &m_Mesh = GetComponent<MeshComponent>();
    m_Shader = std::make_shared<Shader>("src/Sandbox/shader.vert", "src/Sandbox/shader.frag");

    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    m_Mesh.CreateMesh(vertices, m_Shader);
}

void Azimuth::PlayerMovement::OnUpdate()
{
    GetComponent<TransformComponent>().Position = glm::vec3(glm::sin(glfwGetTime()), 0.0f, 0.0f);
}