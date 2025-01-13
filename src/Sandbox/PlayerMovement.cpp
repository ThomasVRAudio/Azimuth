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
    float sine = glm::sin(Time::time() * 5.0f);
    GetComponent<TransformComponent>().Position = glm::vec3(sine, 0.0f, 0.0f);
}
