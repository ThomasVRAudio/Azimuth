#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/Components/IComponent.h>

namespace Azimuth
{
    class GameComponent
    {
    };

    class Material : public GameComponent
    {
    public:
        virtual ~Material() = default;

        virtual void SetUniformVec3(const std::string &name, const glm::vec3 &vec) const = 0;
    };

    class Transform : public GameComponent
    {
    public:
        glm::vec3 Position = glm::vec3(0.0f);
        glm::vec3 Rotation = glm::vec3(0.0f);
        glm::vec3 Scale = glm::vec3(1.0f);
        virtual ~Transform() = default;
        virtual glm::mat4 GetTransform() = 0;
    };
}