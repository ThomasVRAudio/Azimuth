#include "PlayerMovement.h"

void Azimuth::PlayerMovement::OnStart()
{
    AddComponent<TransformComponent>();
    AddComponent<MeshComponent>();

    MeshComponent &m_Mesh = GetComponent<MeshComponent>();
    m_Shader = std::make_shared<Shader>("src/Sandbox/shader.vert", "src/Sandbox/shader.frag");

    m_Mesh.CreateMesh(PRIMITIVE_TRIANGLE, m_Shader);
}

void Azimuth::PlayerMovement::OnUpdate()
{
    float speed = glm::sin(glfwGetTime()) * 0.5f + 0.5f;
    float sine = glm::sin(glfwGetTime() * speed * 2.0f);
    GetComponent<TransformComponent>().Position = glm::vec3(sine, 0.0f, 0.0f);
}
