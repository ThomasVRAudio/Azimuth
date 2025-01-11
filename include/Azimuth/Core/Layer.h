#pragma once

namespace Azimuth
{
    class Layer
    {
    public:
        virtual void Init() {};
        virtual void OnStart() = 0;
        virtual void OnUpdate() = 0;
    };

}