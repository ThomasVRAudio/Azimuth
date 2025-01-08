#pragma once

#include <dependencies/glm/glm.hpp>

namespace Azimuth
{
    using ComponentBitType = std::uint8_t;
    const ComponentBitType MAX_COMPONENTS = 32;

    struct IComponent
    {
    };

    struct TransformComponent : public IComponent
    {
        glm::vec3 Position;
    };

    struct MeshComponent : public IComponent
    {
    };

    struct AudioComponent : public IComponent
    {
    };

}