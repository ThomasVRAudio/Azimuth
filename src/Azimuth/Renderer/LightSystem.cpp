#include <Azimuth/Renderer/LightSystem.h>

namespace Azimuth
{

    void LightSystem::Init(ECSManager *ECS, std::vector<std::shared_ptr<Light>> *lights)
    {
        m_Lights = lights;
        m_ECS = ECS;
    };

    void LightSystem::UpdateLights()
    {
        m_Lights->clear();
        std::vector<Light *> lights;
        for (auto &entity : m_Entities)
        {
            LightComponent &lightComponent = m_ECS->GetComponent<LightComponent>(entity);
            TransformComponent &transform = m_ECS->GetComponent<TransformComponent>(entity);
            std::shared_ptr<Light> light = std::make_shared<Light>(Light{
                .Transform = &transform,
                .Color = &lightComponent.Color,
                .Type = &lightComponent.Type});

            m_Lights->emplace_back(light);
        }
    }
}