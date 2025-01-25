#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    struct Light
    {
        TransformComponent *Transform;
        glm::vec3 *Color;
        LightType *Type;
    };

    class LightSystem : public System
    {
    public:
        LightSystem() = default;
        void Init(ECSManager *ECS, std::vector<std::shared_ptr<Light>> *lights);
        void UpdateLights();

    private:
        ECSManager *m_ECS;
        std::vector<std::shared_ptr<Light>> *m_Lights;
    };
}