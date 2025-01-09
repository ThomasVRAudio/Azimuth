#include <Azimuth/Editor/EditorLayer.h>

namespace Azimuth
{

    void EditorLayer::OnStart()
    {
        ECSManager &ECS = ECSManager::getInstance();

        ComponentMask mask;
        mask.set(ECS.GetComponentBitType<TransformComponent>(), true);
        m_RenderSystem = ECS.RegisterSystem<RenderSystem>();
        ECS.SetSystemComponentMask<RenderSystem>(mask);

        m_RenderSystem->Init();
    }

    void EditorLayer::OnUpdate()
    {
        m_RenderSystem->DrawScene();
    }

}