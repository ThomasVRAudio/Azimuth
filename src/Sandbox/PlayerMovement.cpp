#include "PlayerMovement.h"

void Azimuth::PlayerMovement::OnStart()
{
    AddComponent<MeshComponent>();
    AddComponent<MaterialComponent>();

    MeshComponent &mesh = GetComponent<MeshComponent>();
    auto shader = Shader("src/Sandbox/shader.vert", "src/Sandbox/shader.frag");
    MaterialComponent &mat = GetComponent<MaterialComponent>();

    mesh.CreateMesh(GEOMETRY_TRIANGLE);
    mat.CreateMaterial(std::make_shared<Shader>(shader));
}

void Azimuth::PlayerMovement::OnUpdate()
{
    auto mousePos = Input::GetMouseXYScreen();
    auto &position = GetComponent<TransformComponent>().Position;
    position.x = glm::sin(Time::time() * glm::radians(360.0f)) * 1.0f;
}
