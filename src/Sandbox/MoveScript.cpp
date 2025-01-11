#include "MoveScript.h"
#include <dependencies/GLFW/glfw3.h>

void Azimuth::MoveScript::OnStart()
{
    AddComponent<TransformComponent>();
    m_Transform = GetComponent<TransformComponent>();
}

void Azimuth::MoveScript::OnUpdate()
{
    m_Transform.Position = glm::vec3(glm::sin(glfwGetTime()), 0.0f, 0.0f);
    print("x: " << m_Transform.Position.x);
}