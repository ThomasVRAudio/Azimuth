#pragma once
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>

namespace Azimuth
{
    /*
        Init
        BeginScene // set camera
        EndScene // Draw everything
        DrawObject // Add to things to draw

    */

    class RenderSystem : public System
    {
    public:
        RenderSystem() = default;
        void Init();
        void DrawScene();
    };
}