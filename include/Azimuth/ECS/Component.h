#pragma once
#include <Azimuth/ECS/Components/MaterialComponent.h>
#include <Azimuth/ECS/Components/MeshComponent.h>
// #include <Azimuth/ECS/Components/ScriptComponent.h>
#include <Azimuth/ECS/Components/AudioComponent.h>
#include <Azimuth/ECS/Components/TransformComponent.h>
#include <Azimuth/ECS/Components/TagComponent.h>
#include <Azimuth/ECS/Components/LightComponent.h>

// temp
namespace Azimuth
{
    class TestScript
    {
    public:
        virtual void OnStart() = 0;
        virtual void OnUpdate() = 0;
    };
}