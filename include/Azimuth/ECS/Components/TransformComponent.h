#pragma once
#include <Azimuth/ECS/Components/IComponent.h>
#include <Azimuth/Common.h>
#include <Azimuth/API/GameComponent.h>

namespace Azimuth
{
    struct TransformComponent : public Transform, public IComponent
    {
        glm::mat4 GetTransform()
        {
            glm::mat4 rotationMatrix = glm::mat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Position) * rotationMatrix * glm::scale(glm::mat4(1.0f), Scale);
        }
    };
}