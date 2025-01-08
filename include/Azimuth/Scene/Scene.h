#pragma once
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Core/Layer.h>

namespace Azimuth
{

    class Scene
    {
    public:
        Scene();
        ECSManager ECS;
    };
}