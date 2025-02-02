#include <Azimuth/Renderer/LightSystem.h>

namespace Azimuth
{

    void LightSystem::Init(ECSManager *ECS)
    {
        m_ECS = ECS;
    };

    void LightSystem::UpdateLights()
    {
        PointLights.clear();
        SpotLights.clear();

        std::vector<Light *> lights;
        for (auto &entity : m_Entities)
        {
            LightComponent &lightComponent = m_ECS->GetComponent<LightComponent>(entity);
            TransformComponent &transform = m_ECS->GetComponent<TransformComponent>(entity);

            std::shared_ptr<Light> light = std::make_shared<Light>(Light{
                .Transform = &transform,
                .Color = &lightComponent.Color,
                .Type = &lightComponent.Type,
                .HDRMultiplier = &lightComponent.HDRMultiplier});

            switch (lightComponent.Type)
            {
            case POINT_LIGHT:
                PointLights.emplace_back(light);
                break;
            case DIRECTIONAL_LIGHT:
                DirectionalLight = light;
                break;
            case SPOT_LIGHT:
                SpotLights.emplace_back(light);
                break;
            }
        }
    }
}
