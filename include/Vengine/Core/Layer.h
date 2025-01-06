#pragma once

namespace Vengine
{
    class Layer
    {
    public:
        virtual void OnStart() const = 0;
        virtual void OnUpdate() const = 0;
    };

}