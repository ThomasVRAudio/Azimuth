#pragma once
#include <Azimuth/ECS/Components/IComponent.h>
#include <Azimuth/Common.h>

struct TransformComponent : public IComponent
{
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
};