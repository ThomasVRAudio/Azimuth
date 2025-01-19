#include "PlayerMovement.h"

void Azimuth::PlayerMovement::OnStart()
{
    AddComponent<TransformComponent>();
    AddComponent<MeshComponent>();

    MeshComponent &mesh = GetComponent<MeshComponent>();
    auto shader = std::make_shared<Shader>("src/Sandbox/shader.vert", "src/Sandbox/shader.frag");

    mesh.CreateMesh(PRIMITIVE_TRIANGLE, shader);
}

void Azimuth::PlayerMovement::OnUpdate()
{
    auto mousePos = Input::GetMouseXYScreen();
    auto &position = GetComponent<TransformComponent>().Position;
    position.x = glm::sin(Time::time() * glm::radians(360.0f)) * 1.0f;
}
