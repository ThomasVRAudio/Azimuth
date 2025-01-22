#pragma once
#include <Azimuth/ECS/Components/IComponent.h>

struct TagComponent : public IComponent
{
    std::string name;
};