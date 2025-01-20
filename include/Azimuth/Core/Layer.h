#pragma once

namespace Azimuth
{
    class Scene;

    class Layer
    {
    public:
        virtual void Init(Scene *scene) {};
        virtual void OnStart() = 0;
        virtual void OnUpdate() = 0;
    };

}