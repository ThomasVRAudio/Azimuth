#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{

    void EditorLayer::Init()
    {
        ECSManager &ECS = ECSManager::GetInstance();

        ComponentMask mask;
        mask.set(ECS.GetComponentBitType<TransformComponent>(), true);
        mask.set(ECS.GetComponentBitType<MeshComponent>(), true);
        m_RenderSystem = ECS.RegisterSystem<RenderSystem>();
        ECS.SetSystemComponentMask<RenderSystem>(mask);

        m_RenderSystem->Init();
    }

    void EditorLayer::OnStart()
    {
    }

    void EditorLayer::OnUpdate()
    {
        m_RenderSystem->DrawScene();
    }

}