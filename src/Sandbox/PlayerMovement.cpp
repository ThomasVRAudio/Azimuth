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
    auto movePosition = glm::vec3(mousePos.x * 2.0f - 1.0f, mousePos.y * 2.0f - 1.0f, 0.0f);
    movePosition.x += glm::sin(Time::time() * glm::radians(360.0f)) * 0.05f;

    GetComponent<TransformComponent>().Position = movePosition;

    if (Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        print("Mouse Right is pressed");
}
