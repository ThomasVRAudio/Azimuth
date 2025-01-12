#pragma once
#include <Azimuth/Azimuth.h>

namespace Azimuth
{

    class PlayerMovement : public MonoScript
    {
    public:
        void OnStart();
        void OnUpdate();

    private:
        std::shared_ptr<Shader> m_Shader;
    };
}