#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    using ComponentBitType = std::uint8_t;
    const ComponentBitType MAX_COMPONENTS = 32;

    using Entity = std::int32_t;
    using ComponentMask = std::bitset<MAX_COMPONENTS>;
    const Entity MAX_ENTITIES = 10000;
}