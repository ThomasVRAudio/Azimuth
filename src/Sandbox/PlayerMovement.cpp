#include "PlayerMovement.h"

void Azimuth::PlayerMovement::OnStart()
{
    AddComponent<TransformComponent>();
    m_Transform = GetComponent<TransformComponent>();
}

void Azimuth::PlayerMovement::OnUpdate()
{
    m_Transform.Position = glm::vec3(glm::sin(glfwGetTime()), 0.0f, 0.0f);
    print("x: " << m_Transform.Position.x);
}