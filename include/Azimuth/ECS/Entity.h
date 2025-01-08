#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    using Entity = std::uint32_t;
    using ComponentMask = std::bitset<MAX_COMPONENTS>;
    const Entity MAX_ENTITIES = 10000;
}