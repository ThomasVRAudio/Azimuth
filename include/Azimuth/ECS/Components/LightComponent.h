#pragma once
#include <Azimuth/ECS/Components/IComponent.h>
#include <Azimuth/Common.h>

namespace Azimuth
{
    enum LightType
    {
        POINT_LIGHT,
        DIRECTIONAL_LIGHT,
        SPOT_LIGHT
    };

    class LightComponent : public IComponent
    {
    public:
        glm::vec3 Color = glm::vec3(1.0f);
        LightType Type = DIRECTIONAL_LIGHT;
        bool IsActive = true;
    };
}