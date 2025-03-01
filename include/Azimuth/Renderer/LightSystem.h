#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/ECS/System.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    struct Light
    {
        TransformComponent *Transform;
        glm::vec3 *Color;
        LightType *Type;
        float *Intensity;
    };

    class LightSystem : public System
    {
    public:
        LightSystem() = default;
        void Init(Scene *scene);
        void UpdateLights();
        std::shared_ptr<Light> DirectionalLight;
        std::vector<std::shared_ptr<Light>> PointLights;
        std::vector<std::shared_ptr<Light>> SpotLights;

    private:
        Scene *m_Scene;
    };
}