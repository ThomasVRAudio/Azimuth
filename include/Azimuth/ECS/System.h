#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/Entity.h>

namespace Azimuth
{
    class System
    {
    public:
        std::set<Entity> m_Entities;
    };
}