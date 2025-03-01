#include <Azimuth/Renderer/LightSystem.h>

namespace Azimuth
{

    void LightSystem::Init(Scene *scene)
    {
        m_Scene = scene;
    };

    void LightSystem::UpdateLights()
    {
        PointLights.clear();
        SpotLights.clear();

        std::vector<Light *> lights;
        for (auto &entity : m_Entities)
        {
            ECSManager *ECS = m_Scene->GetECSManager();
            LightComponent &lightComponent = ECS->GetComponent<LightComponent>(entity);
            TransformComponent &transform = ECS->GetComponent<TransformComponent>(entity);

            std::shared_ptr<Light> light = std::make_shared<Light>(Light{
                .Transform = &transform,
                .Color = &lightComponent.Color,
                .Type = &lightComponent.Type,
                .Intensity = &lightComponent.Intensity});

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
